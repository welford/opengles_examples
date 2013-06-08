#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bcm_host.h"

#include "wrapper_gl.h"
//#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>

#include <platform.h>
#include <shader_gl.h>

static const float vertices[] = {
	//0
	-0.5f, -0.5f, 0.0f, ///VVV
	 1.0f,  1.0f, 1.0f, ///CCC	
	-0.5f, -0.5f,		///TX
	 //1
	 0.5f, -0.5f, 0.0f, ///VVV
	 0.0f,  0.0f, 1.0f, ///CCC
	 1.5f, -0.5f,		///TX
	 //2
	 +0.5f, 0.5f, 0.0f, ///VVV
	 0.0f,  0.0f, 1.0f, ///CCC
	 1.5f,  1.5f,		///TX
	 //3
	-0.5f,  0.5f, 0.0f, ///VVV
	 1.0f,  0.0f, 0.0f, ///CCC
	-0.5f,  1.5f		///TX
};

static unsigned char indices[] = {	0, 1, 3, 2 };

static const char* pVertexShader = "\
attribute vec4 position;\n\attribute vec3 colour;\n\attribute vec2 texture;\n\varying vec3 vColour;\n\varying vec2 vTexture;\n\void main() {\n\	gl_Position = vec4(position.xyz,position.w);\n\	vColour = colour;\n\	vTexture = texture;\n\}\n";

static const char* pFragmentShader = "\
varying vec3 vColour;\n\
varying vec2 vTexture;\n\
uniform sampler2D tex;\
void main() {\n\		//gl_FragColor = vec4(vColour,1.0);\n\
		 gl_FragColor = texture2D(tex, vTexture.st);\n\
}\n";

#define WIDTH 16
#define HEIGHT 16
#define MAX_TEXTURES 5

int main ()
{
	int ab=0, eab=0, w=0, h=0, uTex=0;
	unsigned int position_attrib_idx = 0;
	unsigned int colour_attrib_idx = 1;
	unsigned int texture_attrib_idx = 2;
	int utex = 0, active_texture=0;
	unsigned char * pTexture = 0;
	STextureDescriptor desc = {TEX_2D, TEX_FMT_RGB, WIDTH, HEIGHT, 0, 0, GEN_MIPMAP};
	STextureParam param[] ={
		//tex 0
		{ MIN_FILTER, PARAM_NEAREST},
		{ MAG_FILTER, PARAM_NEAREST},
		{ WRAP_S, PARAM_REPEAT},
		{ WRAP_T, PARAM_REPEAT},
		// tex 1
		{ MIN_FILTER, PARAM_LINEAR},
		{ MAG_FILTER, PARAM_LINEAR},
		{ WRAP_S, PARAM_REPEAT},
		{ WRAP_T, PARAM_REPEAT},
		// tex 2
		{ MIN_FILTER, PARAM_LINEAR},
		{ MAG_FILTER, PARAM_LINEAR},
		{ WRAP_S, PARAM_REPEAT},
		{ WRAP_T, PARAM_CLAMP_TO_EDGE},
		// tex 3
		{ MIN_FILTER, PARAM_LINEAR},
		{ MAG_FILTER, PARAM_LINEAR},
		{ WRAP_S, PARAM_CLAMP_TO_EDGE},
		{ WRAP_T, PARAM_CLAMP_TO_EDGE},
		// tex 4
		{ MIN_FILTER, PARAM_NEAREST},
		{ MAG_FILTER, PARAM_NEAREST},
		{ WRAP_S, PARAM_CLAMP_TO_EDGE},
		{ WRAP_T, PARAM_CLAMP_TO_EDGE},
	};
	SPixelData pixelData = {PX_RGB, PX_UNSIGNED_BYTE, 0, 0};
	STextureObject tex[MAX_TEXTURES];

	//GL stuff
	SShader vertexShader;
	SShader fragmentShader;
	SShaderProgram program;

	CPlatform platform;
	Create(&platform, "", 2, 1, 640, 640, 8, 8, 8, 8, 16, 8, 0);	

	pTexture = (unsigned char*)malloc(WIDTH * HEIGHT * 3);
	for(h=0;h<HEIGHT;h++)
		for(w=0;w<WIDTH;w++)
		{
			pTexture[(h*WIDTH*3)+(w*3)+0] = (255*w)/(WIDTH);			//R
			pTexture[(h*WIDTH*3)+(w*3)+1] = (255*h)/(HEIGHT);			//G
			pTexture[(h*WIDTH*3)+(w*3)+2] = (255*(w+h))/(WIDTH+HEIGHT);	//B
		}	
	pixelData.pData = (void*)pTexture;

	ActiveTexture(UNIT_0);
	//tex 0
	if(CreateBaseTexture(&tex[0], &desc))
	{		
		UpdateTextureData(&tex[0], &pixelData);
		SetTextureParams(&tex[0], param, 4);		
	}
	//tex 1
	if(CreateBaseTextureData(&tex[1], &desc, &pixelData))
		SetTextureParams(&tex[1], param+4, 4);		
	//tex 2
	if(CreateBaseTextureData(&tex[2], &desc, &pixelData))
		SetTextureParams(&tex[2], param+8, 4);		
	//tex 3
	if(CreateBaseTextureData(&tex[3], &desc, &pixelData))
		SetTextureParams(&tex[3], param+12, 4);		
	//tex 4
	if(CreateBaseTextureData(&tex[4], &desc, &pixelData))
		SetTextureParams(&tex[4], param+16, 4);		
	free(pTexture);

	//-------------------
	//setup the shaders
	CreateShader(&vertexShader, VERT, &pVertexShader, 1);	
	CreateShader(&fragmentShader, FRAG, &pFragmentShader, 1);	
	
	CreateShaderProgram(&program);
	AddShaderToProgram(&program, &vertexShader);
	AddShaderToProgram(&program, &fragmentShader);
	SetAttributeLocation(&program, position_attrib_idx, "position");
	SetAttributeLocation(&program, colour_attrib_idx, "colour");
	SetAttributeLocation(&program, texture_attrib_idx, "texture");
	LinkShaderProgram(&program);	
	uTex = GetUniformLocation(&program, "tex");
	
	ab = CreateBufferUsingData(BUFFER_ARRAY, BUFFER_STATIC, sizeof(vertices), vertices);
	eab = CreateBufferUsingData(BUFFER_ELEMENTS, BUFFER_STATIC, sizeof(indices), indices);

	//-------------------
	//tell openGL how to interperet the data
	BindBuffer(BUFFER_ELEMENTS, eab);
	BindBuffer(BUFFER_ARRAY, ab);
	SVertexAttribute attr[] = {
		{ab, position_attrib_idx, 3, STREAM_FLOAT, 0, 32, 0, 0},//P
		{ab, colour_attrib_idx, 3, STREAM_FLOAT, 0, 32, 12, 0},	//C
		{ab, texture_attrib_idx, 2, STREAM_FLOAT, 0, 32, 24, 0}	//T
	};
	SetAttributeFormat( attr, 3, 0);
	//set rendering states
	EnableCulling(1);	
	SetClearColour(0, 0, 0, 0.0f); //alpha to 0, should make triangle appear over console

	Start(&program); 
	SetTextureUnitByLocation(utex, 0);
	BindTexture(tex+active_texture);	
	printf("Texture Index %d, id %d\n",active_texture,tex[active_texture]);			
	while (!IS_BUTTON_PRESSED(platform.m_keyboard.key[KB_ESC]))
	{
		Tick(&platform);
		if(IS_BUTTON_TOGGLE_PRESSED(platform.m_keyboard.key[KB_SPACE]))
		{
			if(++active_texture >= MAX_TEXTURES)
				active_texture = 0;
			BindTexture(tex+active_texture);				
			printf("Texture Index %d, id %d\n",active_texture,tex[active_texture]);			
		}
		ClearScreenBuffer(COLOR_BIT);		
		Draw(DRAW_TRIANGLE_STRIP, ELEMENT_BYTE, sizeof(indices)/sizeof(unsigned char), 0);					
		SwapBuffers(&platform);		
	}
	Stop();	
	DeleteShaderProgram(&program);
	DeleteShader(&vertexShader);
	DeleteShader(&fragmentShader);
	Close(&platform);
}