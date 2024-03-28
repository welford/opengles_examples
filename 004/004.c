#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bcm_host.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <platform.h>
#include <shader_gl.h>

/*
simple example for rendering a triangle
*/

//triangle's vertices
static const float vertices[] = {
	//0
	-0.5f, -0.5f, 0.0f, //P
	 //1
	+0.5f, -0.5f, 0.0f,	//P
	 //2
	0.0f, +0.5f, 0.0f,	//P
};


static const char* pVertexShader = "\
attribute vec4 position;\n\
void main() {\n\
	gl_Position = position;\n\
}\n";

static const char* pFragmentShader = "\
precision mediump float;\n\
uniform vec3 uColour;\n\
void main() {\n\
		gl_FragColor = vec4(uColour,1.0);\n\
}\n";
//static variables 
GLuint ab=0;	

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

int main ()
{
	int i=0;
	int ab=0;
	float clr[3] = {0.0f, 0.3f, 0.6f};
	float add[3] = {0.001f, 0.002f, 0.003f};
	unsigned int position_attrib_idx = 0;

	//GL stuff
	SShader vertexShader;
	SShader fragmentShader;
	SShaderProgram program;

	CPlatform platform;
	Create(&platform, "", 2, 0, 1024, 768, 8, 8, 8, 8, 16, 8, 0);	

	//-------------------
	//setup the shaders
	CreateShader(&vertexShader, VERT, &pVertexShader, 1);	
	CreateShader(&fragmentShader, FRAG, &pFragmentShader, 1);	
	
	CreateShaderProgram(&program);
	AddShaderToProgram(&program, &vertexShader);
	AddShaderToProgram(&program, &fragmentShader);
	SetAttributeLocation(&program, position_attrib_idx, "position");	//needs to be calleb before linking
	LinkShaderProgram(&program);	
	
	glGenBuffers(1, &ab);		//array buffer	
	//fill buffers
	glBindBuffer(GL_ARRAY_BUFFER, ab);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//-------------------
	//tell openGL how to interperet the data
	glBindBuffer(GL_ARRAY_BUFFER, ab);
	glEnableVertexAttribArray(0);	
	glVertexAttribPointer(position_attrib_idx, 3, GL_FLOAT, GL_FALSE, 12, 0);

	//set rendering states
	glEnable(GL_CULL_FACE);	
	glClearColor(0, 0, 0, 0.0f); //alpha to 0, should make triangle appear over console
	Start(&program); //even when glDeleteProgram is called the program won't be deleted until it is out of use
	while (!IS_BUTTON_PRESSED(platform.m_keyboard.key[KB_ESC]) && !platform.m_quit)
	{
		Tick(&platform);
		glClear(GL_COLOR_BUFFER_BIT);
		SetVec3ByName(&program, "uColour", clr);
		glDrawArrays( GL_TRIANGLES, 0, 3);	
		SwapBuffers(&platform);

		for(i=0;i<3;i++)
		{
			clr[i]+=add[i];
			if(clr[i]>1.0f)add[i]*=-1.0f;		
			if(clr[i]<0.0f)add[i]*=-1.0f;
		}
	}
	Stop();	
	DeleteShaderProgram(&program);
	DeleteShader(&vertexShader);
	DeleteShader(&fragmentShader);
	Close(&platform);
}