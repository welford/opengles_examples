#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bcm_host.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <platform.h>
#include <shader_gl.h>

//#define INTERLEAVED

/*
simple example for rendering a triangle
*/

//triangle's vertices
#ifdef INTERLEAVED
static const float vertices[] = {
	//0
	-0.5f, -0.5f, 0.0f, //P
	1.0f, 0.0f, 0.0f,	//C
	 //1
	+0.5f, -0.5f, 0.0f,	//P
	0.0f, 1.0f, 0.0f,	//C
	 //2
	0.0f, +0.5f, 0.0f,	//P
	0.0f, 0.0f, 1.0f,	//C
};
#else
static const float vertices[] = {
	//0
	-0.5f, -0.5f, 0.0f, //P	
	+0.5f, -0.5f, 0.0f,	//P	
	0.0f, +0.5f, 0.0f,	//P

	1.0f, 0.0f, 0.0f,	//C
	0.0f, 1.0f, 0.0f,	//C
	0.0f, 0.0f, 1.0f,	//C
};
#endif


static const char* pVertexShader = "\
attribute vec4 position;\n\attribute vec3 colour;\n\varying vec3 vryColour;\n\void main() {\n\	gl_Position = position;\n\	vryColour = colour;\n\}\n";

static const char* pFragmentShader = "\
varying vec3 vryColour;\n\
void main() {\n\		//gl_FragColor = vec4(1.0);\n\		gl_FragColor = vec4(vryColour,1.0);\n\}\n";
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
	unsigned int colour_attrib_idx = 1;

	//GL stuff
	SShader vertexShader;
	SShader fragmentShader;
	SShaderProgram program;

	CPlatform platform;
	Create(&platform, "", 2, 1, 1024, 768, 8, 8, 8, 8, 16, 8, 0);	
	//Create(&platform, "", 2, 1, 1024, 768, 0, 0, 0, 0, 16, 8, 0);

	//-------------------
	//setup the shaders
	CreateShader(&vertexShader, VERT, &pVertexShader, 1);	
	CreateShader(&fragmentShader, FRAG, &pFragmentShader, 1);	
	
	//*
	CreateShaderProgram(&program);
	AddShaderToProgram(&program, &vertexShader);
	AddShaderToProgram(&program, &fragmentShader);
	SetAttributeLocation(&program, position_attrib_idx, "position");	//needs to be calleb before linking
	SetAttributeLocation(&program, colour_attrib_idx, "colour");	//of we require a re link		
	LinkShaderProgram(&program);
	/*/ //or we could just do this, which links the program then queries the locations 
	CreateShaderProgram(&program);
	AddShaderToProgram(&program, &vertexShader);
	AddShaderToProgram(&program, &fragmentShader);	
	LinkShaderProgram(&program);
	position_attrib_idx = GetAttributeLocation(&program, "position");
	colour_attrib_idx = GetAttributeLocation(&program, "colour");
	//*/

	glGenBuffers(1, &ab);		//array buffer	
	//fill buffers
	glBindBuffer(GL_ARRAY_BUFFER, ab);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//-------------------
	//tell openGL how to interperet the data
	glBindBuffer(GL_ARRAY_BUFFER, ab);
	glEnableVertexAttribArray(0);	
	glEnableVertexAttribArray(1);	
	#ifdef INTERLEAVED
	glVertexAttribPointer(position_attrib_idx, 3, GL_FLOAT, GL_FALSE, 24, 0);
	glVertexAttribPointer(colour_attrib_idx, 3, GL_FLOAT, GL_FALSE, 24, BUFFER_OFFSET(12));
	#else
	glVertexAttribPointer(position_attrib_idx, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(colour_attrib_idx, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(36));
	#endif

	//set rendering states
	glEnable(GL_CULL_FACE);	
	Start(&program); //even when glDeleteProgram is called the program won't be deleted until it is out of use
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