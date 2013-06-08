#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bcm_host.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <platform.h>
#include <shader_gl.h>
#include <wrapper_gl.h>

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

#define RENDER_WIDTH_HEIGHT 128

int main ()
{
	int ab=0;
	SFramebufferObject fbo;	

	//GL stuff
	SShader vertexShader;
	SShader fragmentShader;
	SShaderProgram program;
	//textures to render to
	STextureObject texRGB, texDepth;
	//descriptors for the textures
	STextureDescriptor descRGB = {TEX_2D, TEX_FMT_RGB, RENDER_WIDTH_HEIGHT, RENDER_WIDTH_HEIGHT, 0, 0, DONT_GEN_MIPMAP};
	STextureDescriptor descDepth = {TEX_2D, TEX_FMT_DEPTH, RENDER_WIDTH_HEIGHT, RENDER_WIDTH_HEIGHT, 0, 0, DONT_GEN_MIPMAP};
	STextureParam param[] ={
		{ MIN_FILTER, PARAM_NEAREST},
		{ MAG_FILTER, PARAM_NEAREST},
		{ WRAP_S, PARAM_CLAMP_TO_EDGE},
		{ WRAP_T, PARAM_CLAMP_TO_EDGE},
	};

	CPlatform platform;
	Create(&platform, "", 2, 1, 320, 240, 8, 8, 8, 8, 24, 0, 0);	
	//setup the textures 
	CreateBaseTexture(&texRGB, &descRGB);	
	SetTextureParams(&texRGB, param, 4);

	CreateBaseTexture(&texDepth, &descDepth);	
	SetTextureParams(&texDepth, param, 4);

	//setup framebuffers
	CreateFrameBuffer(&fbo);
	AddTextureRenderBuffer(&fbo, &texRGB, ATT_CLR0);	
	AddTextureRenderBuffer(&fbo, &texDepth, ATT_DEPTH);
	CheckFrameBuffer(&fbo);
		
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
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays( GL_TRIANGLES, 0, 3);	
		SwapBuffers(&platform);
	}
	Stop();
	DeleteShaderProgram(&program);
	DeleteShader(&vertexShader);
	DeleteShader(&fragmentShader);
	Close(&platform);
}