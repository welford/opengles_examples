#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <platform.h>
#include <shader_gl.h>

#include <wrapper_gl.h>
#include <transforms.h>
#include <math.h>

static const float vertices[] = {
	 0,0,0,			//P
	 1.0f,0.3f,0.3f,//C
	 1.0f,0,0,		//P
	 1.0f,0.3f,0.3f,//C
	 0,0,0,			//P
	 0.3f,1.0f,0.3f,//C
	 0,1.0f,0,		//P
	 0.3f,1.0f,0.3f,//C
	 0,0,0,			//P
	 0.3f,0.3f,1.0f,//C
	 0,0,1.0f,		//P
	 0.3f,0.3f,1.0f,//C
};

static unsigned char indices[] = {	0, 1, 2, 3, 4, 5 };


static const char* pVertexShader = "\
attribute vec4 position;\n\
attribute vec3 colour;\n\
uniform mat4 mvp;\n\
\n\
varying vec3 vColour;\n\
void main() {\n\
	gl_Position = mvp *  vec4(position.xyz,1.0);\n\
	vColour = colour;\n\
}\n";

static const char* pFragmentShader = "\
precision mediump float;\n\
varying vec3 vColour;\n\
void main() {\n\
		gl_FragColor = vec4(vColour,1.0);\n\
}\n";


int main ()
{
	int ab=0, eab=0, uMvp=0, i=0;
	unsigned int position_attrib_idx = 0;
	unsigned int colour_attrib_idx = 1;

	//camera stuff
	float camera_position[] = {0, 0, -2.0f};
	float latitude = 1.0f, longitude = 0.0f;
	SMatrix44 rotLat,rotLong, mvp, projection;

	//transform stuff	
	STransform transform;

	//GL stuff
	SShader vertexShader;
	SShader fragmentShader;
	SShaderProgram program;

	CPlatform platform;
	Create(&platform, "", 2, 0, 640, 640, 8, 8, 8, 8, 16, 8, 0);	

	//-------------------
	//setup the shaders
	CreateShader(&vertexShader, VERT, &pVertexShader, 1);	
	CreateShader(&fragmentShader, FRAG, &pFragmentShader, 1);	
	
	CreateShaderProgram(&program);
	AddShaderToProgram(&program, &vertexShader);
	AddShaderToProgram(&program, &fragmentShader);
	SetAttributeLocation(&program, position_attrib_idx, "position");
	SetAttributeLocation(&program, colour_attrib_idx, "colour");
	LinkShaderProgram(&program);	
	uMvp = GetUniformLocation(&program, "mvp");

	ab = CreateBufferUsingData(BUFFER_ARRAY, BUFFER_STATIC, sizeof(vertices), vertices);
	eab = CreateBufferUsingData(BUFFER_ELEMENTS, BUFFER_STATIC, sizeof(indices), indices);

	//-------------------
	//tell openGL how to interperet the data
	BindBuffer(BUFFER_ELEMENTS, eab);
	BindBuffer(BUFFER_ARRAY, ab);
	SVertexAttribute attr[] = {
		{ab, position_attrib_idx, 3, STREAM_FLOAT, 0, 24, 0, 0},//P
		{ab, colour_attrib_idx, 3, STREAM_FLOAT, 0, 24, 12, 0},	//C
	};
	SetAttributeFormat( attr, 2, 0);

	//setup camera stuff
	CreateProjectionMatrix(&projection, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 10.0f);
	//set rendering states
	EnableCulling(1);
	SetClearColour(0, 0, 0, 0.0f);

	//start the shader
	Start(&program); 
	TransformInitialise(&transform, 10);	
	while (!IS_BUTTON_PRESSED(platform.m_keyboard.key[KB_ESC]) && !platform.m_quit)
	{
		Tick(&platform);
		if(IS_BUTTON_PRESSED(platform.m_keyboard.key[KB_UP]))
			latitude += 90.0f * 0.001f;
		if(IS_BUTTON_PRESSED(platform.m_keyboard.key[KB_DOWN]))
			latitude -= 90.0f * 0.001f;
		
		if(IS_BUTTON_PRESSED(platform.m_keyboard.key[KB_LEFT]))
			longitude += 90.0f * 0.001f;
		if(IS_BUTTON_PRESSED(platform.m_keyboard.key[KB_RIGHT]))//r
			longitude -= 90.0f * 0.001f;

		ClearScreenBuffer(COLOR_BIT);
		Mtx44Identity(&rotLat);
		Mtx44Identity(&rotLong);
		Mtx44Rotate(&rotLat, latitude, 1, 0, 0);
		Mtx44Rotate(&rotLong, longitude, 0, 1, 0);
		TransformPush(&transform);
			TransformTranslate(&transform, camera_position[0], camera_position[1], camera_position[2]);
			TransformMultiply(&transform, &rotLat);
			TransformMultiply(&transform, &rotLong);
			TransformPush(&transform);
				Mtx44MultiplyMtx44(&mvp, &projection, TransformGetCurrentMatrix(&transform));
				SetMtx44ByLocation(uMvp, mvp.data);
				Draw(DRAW_LINES, ELEMENT_BYTE, sizeof(indices)/sizeof(unsigned char), 0);
			TransformPop(&transform);
		TransformPop(&transform);

		SwapBuffers(&platform);		
	}
	Stop();	
	DeleteShaderProgram(&program);
	DeleteShader(&vertexShader);
	DeleteShader(&fragmentShader);
	Close(&platform);
}