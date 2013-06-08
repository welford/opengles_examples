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
	 +0.5f, -0.5f, 0.0f,//P
	 //2
	 0.0f, +0.5f, 0.0f, //P
};


static const char* pVertexShader = "\
attribute vec4 position;\n\void main() {\n\	gl_Position = position;\n\}\n\
";

static const char* pFragmentShader = "\
	void main() {\n\	    gl_FragColor = vec4(1.0);\n\	}\n\
";
//static variables 
GLuint ab=0;	

int main ()
{
	int i=0;
	int ab=0,vao=0;
	float clr[3] = {0.0f, 0.3f, 0.6f};
	float add[3] = {0.001f, 0.002f, 0.003f};

	//GL stuff
	SShader vertexShader;
	SShader fragmentShader;
	SShaderProgram program;

	CPlatform platform;
	Create(&platform, "", 2, 1, 320, 240, 1, 0, 0, 0, 0, 0, 0);	
	
	//setup the shaders
	CreateShader(&vertexShader, VERT, &pVertexShader, 1);	
	CreateShader(&fragmentShader, FRAG, &pFragmentShader, 1);	
	
	CreateShaderProgram(&program);
	AddShaderToProgram(&program, &vertexShader);
	AddShaderToProgram(&program, &fragmentShader);
	SetAttributeLocation(&program, 0, "position");
	LinkShaderProgram(&program);

	glGenBuffers(1, &ab);		//array buffer	
	//fill buffers
	glBindBuffer(GL_ARRAY_BUFFER, ab);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	//bind vao and ste it's data
	glBindBuffer(GL_ARRAY_BUFFER, ab);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); ///PPP
	glEnableVertexAttribArray(0);	

	//set rendering states
	glEnable(GL_CULL_FACE);
	glClearColor(0,0,0,0);
	Start(&program); //even when glDeleteProgram is called the program won't be deleted untilit7s out of use

	while (!IS_BUTTON_PRESSED(platform.m_keyboard.key[KB_ESC]))
	{
		Tick(&platform);
		glClearColor(clr[0], clr[1], clr[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
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