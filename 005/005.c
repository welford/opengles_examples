#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bcm_host.h"

#include "wrapper_gl.h"
//#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>

#include <platform.h>
#include <shader_gl.h>


/*
simple example for rendering a triangle
*/

//triangle's vertices
static const float vertices[] = {
	//0
	-0.5f, -0.5f, 0.0f, //P
	1.0f,  0.0f, 0.0f, //C
	 //1
	+0.5f, -0.5f, 0.0f,	//P
	0.0f,  1.0f, 0.0f, //C
	 //2
	0.0f, +0.5f, 0.0f,	//P
	0.0f,  0.0f, 1.0f, //C
};


static const char* pVertexShader = "\
attribute vec4 position;\n\attribute vec3 colour;\n\varying vec3 vColour;\n\void main() {\n\	gl_Position = position;\n\	vColour = colour;\n\}\n";

static const char* pFragmentShader = "\
varying vec3 vColour;\n\
void main() {\n\		gl_FragColor = vec4(vColour,1.0);\n\}\n";

int main ()
{
	int i=0;
	int ab=0;	
	unsigned int position_attrib_idx = 0;
	unsigned int colour_attrib_idx = 1;

	//GL stuff
	SShader vertexShader;
	SShader fragmentShader;
	SShaderProgram program;

	CPlatform platform;
	Create(&platform, "", 2, 1, 1024, 768, 8, 8, 8, 8, 16, 8, 0);	

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
	
	ab = CreateBufferUsingData(BUFFER_ARRAY, BUFFER_STATIC, sizeof(vertices), vertices);
	//-------------------
	//tell openGL how to interperet the data
	BindBuffer(BUFFER_ARRAY, ab);

	SVertexAttribute attr[] = {
		{ab, position_attrib_idx, 3, STREAM_FLOAT, 0, 24, 0, 0},//P
		{ab, colour_attrib_idx, 3, STREAM_FLOAT, 0, 24, 12, 0}	//C
	};
	SetAttributeFormat( attr, 2, 0);
	//set rendering states
	EnableCulling(1);	
	SetClearColour(0, 0, 0, 0.0f); //alpha to 0, should make triangle appear over console

	Start(&program); 
	while (!IS_BUTTON_PRESSED(platform.m_keyboard.key[KB_ESC]))
	{
		Tick(&platform);
		ClearScreenBuffer(COLOR_BIT);
		DrawArray(DRAW_TRIANGLES, 3, 0);
		SwapBuffers(&platform);		
	}
	Stop();	
	DeleteShaderProgram(&program);
	DeleteShader(&vertexShader);
	DeleteShader(&fragmentShader);
	Close(&platform);
}