#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <platform.h>
#include <shader_gl.h>

#include <wrapper_gl.h>
#include <transforms.h>
#include <math.h>

#define DIM 16 //if you are going to change this remember to 
				//generate mipmaps for all levels of 4th texture
				//either automatically or manually

#define WIDTH DIM
#define HEIGHT DIM
#define MAX_TEXTURES 5

/*
simple mipmap example
*/

#define SCALE 100.0f
static const float vertices[] = {
	//0
	-1.0f*SCALE, 0.0f, -1.0f*SCALE, //P
	-1.0f*SCALE, -1.0f*SCALE,		//T
	 //1
	 1.0f*SCALE,  0.0f, -1.0f*SCALE, //P
	 1.0f*SCALE, -1.0f*SCALE,		//T
	 //2
	 1.0f*SCALE, 0.0f, 1.0f*SCALE, //P
	 1.0f*SCALE,  1.0f*SCALE,		//T
	 //3
	-1.0f*SCALE,  0.0f, 1.0f*SCALE, //P
	-1.0f*SCALE,  1.0f*SCALE		//T
};

static unsigned char indices[] = {	0, 3, 1, 2 };


static const char* pVertexShader = "\
attribute vec4 position;\n\attribute vec2 texture;\n\uniform mat4 mvp;\n\\n\varying vec2 vtex;\n\void main() {\n\	vtex = texture;\n\	gl_Position = mvp *  vec4(position.xyz,1.0);\n\}\n";

static const char* pFragmentShader = "\
uniform sampler2D tex;\
varying vec2 vtex;\n\
void main() {\n\	gl_FragColor = texture2D(tex, vtex.st);\n\
}\n";

//make sure to keep the order the same in here
//otherwise it'll mess with the debug print below
//min filters are every 4th element
static const char* sFilters[] = {"NEAREST",
"LINEAR",
"NEAREST_MIPMAP_NEAREST",
"LINEAR_MIPMAP_NEAREST",
"NEAREST_MIPMAP_LINEAR",
"LINEAR_MIPMAP_LINEAR"};

//You can change them if you want, but you'll have to chnage the debug print out
STextureParam param[] ={
	//tex 0
	{ MIN_FILTER, PARAM_NEAREST},
	{ MAG_FILTER, PARAM_NEAREST},
	{ WRAP_S, PARAM_REPEAT},
	{ WRAP_T, PARAM_REPEAT},
	// tex 1
	{ MIN_FILTER, PARAM_LINEAR_MIPMAP_NEAREST},
	{ MAG_FILTER, PARAM_NEAREST},
	{ WRAP_S, PARAM_REPEAT},
	{ WRAP_T, PARAM_REPEAT},
	// tex 2
	{ MIN_FILTER, PARAM_NEAREST_MIPMAP_LINEAR},
	{ MAG_FILTER, PARAM_NEAREST},
	{ WRAP_S, PARAM_REPEAT},
	{ WRAP_T, PARAM_REPEAT},
	// tex 3
	{ MIN_FILTER, PARAM_LINEAR_MIPMAP_LINEAR},
	{ MAG_FILTER, PARAM_NEAREST},
	{ WRAP_S, PARAM_REPEAT},
	{ WRAP_T, PARAM_REPEAT},
	// tex 4
	//{ MIN_FILTER, PARAM_LINEAR_MIPMAP_LINEAR},
	//{ MAG_FILTER, PARAM_LINEAR},
	{ MIN_FILTER, PARAM_NEAREST},
	{ MAG_FILTER, PARAM_NEAREST},
	{ WRAP_S, PARAM_REPEAT},
	{ WRAP_T, PARAM_REPEAT},
};

int main ()
{
	int ab=0, eab=0, uMvp=0;
	unsigned int h=0,w=0;
	unsigned int newWidth = 0, newHeight = 0;
	unsigned int position_attrib_idx = 0;
	unsigned int texture_attrib_idx = 1;
	//camera stuff
	float camera_position[] = {0, 0, -10.0f};
	SMatrix44 mvp, projection, rotLat, rotLong;
	STextureObject tex[MAX_TEXTURES];

	int black=0;
	//these will be alloced and filled with the colours their name suggests
	unsigned char * pTexture = 0;
	unsigned char * pTextureRed = 0;
	unsigned char * pTextureGreen = 0;
	unsigned char * pTextureBlue = 0;
	unsigned char * pTextureWhite = 0;
	STextureDescriptor desc = {TEX_2D, TEX_FMT_RGB, WIDTH, HEIGHT, 0, 0, GEN_MIPMAP};
	//transform stuff	
	STransform transform;
	//GL stuff
	SShader vertexShader;
	SShader fragmentShader;
	SShaderProgram program;
	CPlatform platform;
	

	Create(&platform, "", 2, 1, 240, 480, 8, 8, 8, 8, 16, 8, 0);	

	// - - - - - - - - - - 
	// create textures
	//in the case of a 16  by 16 texture this will fill in all levels
	 //1x1 is the msallest level for a mipmap
	pTexture = (unsigned char*)malloc(WIDTH * HEIGHT * 3);
	for(h=0;h<HEIGHT;h++)
	{
		for(w=0;w<WIDTH;w++)
		{
			if((w > WIDTH/2 && h< HEIGHT/2) || (w < WIDTH/2 && h> HEIGHT/2))
				black = 0;
			else
				black = 1;

			pTexture[(h*WIDTH*3)+(w*3)+0] = (black)?0:255;	//R
			pTexture[(h*WIDTH*3)+(w*3)+1] = (black)?0:255;	//R
			pTexture[(h*WIDTH*3)+(w*3)+2] = (black)?0:255;	//R
		}	
	}
	newWidth = WIDTH/2;
	newHeight = HEIGHT/2;
	pTextureRed = (unsigned char*)malloc((newWidth) * (newHeight) * 3);
	for(h=0;h<newHeight;h++)
		for(w=0;w<newWidth;w++)
		{
			pTextureRed[(h*newWidth*3)+(w*3)+0] = 255;					//R
			pTextureRed[(h*newWidth*3)+(w*3)+1] = pTextureRed[(h*newWidth*3)+(w*3)+2] = 0; //GB
		}

	newWidth = newWidth/2;
	newHeight = newHeight/2;
	pTextureGreen = (unsigned char*)malloc((newWidth) * (newHeight) * 3);
	for(h=0;h<newHeight;h++)
		for(w=0;w<newWidth;w++)
		{
			pTextureGreen[(h*newWidth*3)+(w*3)+1] = 255;					//G
			pTextureGreen[(h*newWidth*3)+(w*3)+0] = pTextureGreen[(h*newWidth*3)+(w*3)+2] = 0; //RB
		}

	newWidth = newWidth/2;
	newHeight = newHeight/2;
	pTextureBlue = (unsigned char*)malloc((newWidth) * (newHeight) * 3);
	for(h=0;h<newHeight;h++)
		for(w=0;w<newWidth;w++)
		{
			pTextureBlue[(h*newWidth*3)+(w*3)+2] = 255;					//B
			pTextureBlue[(h*newWidth*3)+(w*3)+0] = pTextureBlue[(h*newWidth*3)+(w*3)+1] = 0; //RG
		}

	newWidth = newWidth/2;
	newHeight = newHeight/2;
	pTextureWhite = (unsigned char*)malloc((newWidth) * (newHeight) * 3);
	memset(pTextureWhite, 255, (newWidth) * (newHeight) * 3);
	
	SPixelData data = {PX_RGB, PX_UNSIGNED_BYTE, 0, (void*)pTexture};

	ActiveTexture(UNIT_0);
	//tex 0
	if(CreateBaseTexture(&tex[0], &desc))
	{		
		UpdateTextureData(&tex[0], &data);
		SetTextureParams(&tex[0], param, 4);
	}	
	//tex 1
	if(CreateBaseTextureData(&tex[1], &desc, &data))
		SetTextureParams(&tex[1], param+4, 4);		
	//tex 2
	if(CreateBaseTextureData(&tex[2], &desc, &data))
		SetTextureParams(&tex[2], param+8, 4);		
	//tex 3
	if(CreateBaseTextureData(&tex[3], &desc, &data))
		SetTextureParams(&tex[3], param+12, 4);		

	//tex 4
	//for the final one use a custom mipmap
	desc.genMipMap = DONT_GEN_MIPMAP; //if you are going to set all levels of the mipmap manually then setting this to 
	//false is Ok, if you are not and you only wish to override some levels of the mipmap, set it to true and then
	//
	//in the case of a 16  by 16 texture this will fill in all levels
	//1x1 is the msallest level for a mipmap
	if(CreateBaseTextureData(&tex[4], &desc, &data))
	{
		desc.genMipMap = DONT_GEN_MIPMAP;

		data.pData = (void*)pTextureRed;
		desc.w = WIDTH/2;
		desc.h = HEIGHT/2;
		data.level = 1;
		CreateTextureData(&tex[4], &desc, &data);

		data.pData = (void*)pTextureGreen;
		desc.w = WIDTH/4;
		desc.h = HEIGHT/4;
		data.level = 2;
		CreateTextureData(&tex[4], &desc, &data);		
		
		data.pData = (void*)pTextureBlue;
		desc.w = WIDTH/8;
		desc.h = HEIGHT/8;
		data.level = 3;
		CreateTextureData(&tex[4], &desc, &data);		
		
		data.pData = (void*)pTextureWhite;
		desc.w = WIDTH/16;
		desc.h = HEIGHT/16;
		data.level = 4;
		CreateTextureData(&tex[4], &desc, &data);

		SetTextureParams(&tex[4], param+16, 4);		
	}
	free(pTexture);
	free(pTextureRed);
	free(pTextureGreen);
	free(pTextureBlue);
	free(pTextureWhite);

	//-------------------
	//setup the shaders
	CreateShader(&vertexShader, VERT, &pVertexShader, 1);	
	CreateShader(&fragmentShader, FRAG, &pFragmentShader, 1);	
	
	CreateShaderProgram(&program);
	AddShaderToProgram(&program, &vertexShader);
	AddShaderToProgram(&program, &fragmentShader);
	SetAttributeLocation(&program, position_attrib_idx, "position");
	SetAttributeLocation(&program, texture_attrib_idx, "texture");
	LinkShaderProgram(&program);	
	uMvp = GetUniformLocation(&program, "mvp");

	ab = CreateBufferUsingData(BUFFER_ARRAY, BUFFER_STATIC, sizeof(vertices), vertices);
	eab = CreateBufferUsingData(BUFFER_ELEMENTS, BUFFER_STATIC, sizeof(indices), indices);
	
	//-------------------
	//tell openGL how to interperet the data
	BindBuffer(BUFFER_ELEMENTS, eab);
	BindBuffer(BUFFER_ARRAY, ab);
	SVertexAttribute attr[] = {
		{ab, position_attrib_idx, 3, STREAM_FLOAT, 0, 20, 0, 0},//P
		{ab, texture_attrib_idx, 2, STREAM_FLOAT, 0, 20, 12, 0},//T
	};
	SetAttributeFormat( attr, 2, 0);
	
	//setup camera stuff
	CreateProjectionMatrix(&projection, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f*SCALE + 10.0f);
	//set rendering states
	EnableCulling(1);
	SetClearColour(0, 0, 0, 0);

	//start the shader
	Start(&program); 
	int active_texture = 0;
	BindTexture(&tex[active_texture]);
	printf("Texture Index %d, id %d\n",active_texture,tex[active_texture].handle);
	TransformInitialise(&transform, 10);	
	while (!IS_BUTTON_PRESSED(platform.m_keyboard.key[KB_ESC]))
	{
		Tick(&platform);
		if(IS_BUTTON_TOGGLE_PRESSED(platform.m_keyboard.key[KB_SPACE]))
		{
			if(++active_texture>= MAX_TEXTURES)
				active_texture= 0;
			BindTexture(&tex[active_texture]);
			printf("Texture Index %d, id %d\n",active_texture,tex[active_texture].handle);			
			printf("Texture Min Filter %s \n",sFilters[param[active_texture*4].param - PARAM_NEAREST]);			
		}

		ClearScreenBuffer(COLOR_BIT);
		TransformPush(&transform);
			Mtx44Identity(&rotLat);
			Mtx44Identity(&rotLong);
			Mtx44Rotate(&rotLat, 0.25f*M_PI, 1, 0, 0);
			Mtx44Rotate(&rotLong, 0.25f*M_PI, 0, 1, 0);

			TransformTranslate(&transform, camera_position[0], camera_position[1], camera_position[2]);
			TransformMultiply(&transform, &rotLat);
			TransformMultiply(&transform, &rotLong);
			//TransformPush(&transform);
			Mtx44MultiplyMtx44(&mvp, &projection, TransformGetCurrentMatrix(&transform));
			SetMtx44ByLocation(uMvp, mvp.data);
			Draw(DRAW_TRIANGLE_STRIP, ELEMENT_BYTE, sizeof(indices)/sizeof(unsigned char), 0);		
			//TransformPop(&transform);
		TransformPop(&transform);

		SwapBuffers(&platform);		
	}
	Stop();	
	DeleteShaderProgram(&program);
	DeleteShader(&vertexShader);
	DeleteShader(&fragmentShader);
	Close(&platform);
}