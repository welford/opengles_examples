#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bcm_host.h"

#include "wrapper_gl.h"
//#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>

#include <platform.h>
#include <shader_gl.h>

#define SQUARES 10.0f
#define SQUARE_WH 1.0f
#define STEP_RANGE (2.0f-SQUARE_WH) //0 -> 2-SIZE
//FMT
///VVVCCCNNN
//FMT
///VVVCCCNNN
static const float vertices[] = {
	//0
	 0.0f,  0.0f, 0.0f,		//P
	 1.0f,  1.0f, 1.0f,		//C
	 //1
	 SQUARE_WH,  0.0f, 0.0f,//P
	 1.0f,  1.0f, 1.0f,		//C
	 //2
	 SQUARE_WH,  SQUARE_WH, 0.0f, //P
	 1.0f,  1.0f, 1.0f, ///C
	 //3
	 0.0f,  SQUARE_WH, 0.0f, ///P
	 1.0f,  1.0f, 1.0f, ///C
};

static unsigned char indices[] = {	0, 1, 3, 2 };

static const char* pVertexShader = "\
attribute ve../platform.pi.o c4 position;\n\
attribute vec3 colour;\n\
varying vec3 vColour;\n\
uniform vec3 offset;\n\
void main() {\n\
	gl_Position = vec4(position.xyz+offset,position.w);\n\
	vColour = colour;\n\
}\n";

static const char* pFragmentShader = "\
precision mediump float;\n\
varying vec3 vColour;\n\
void main() {\n\
		//gl_FragColor = vec4(vColour,1.0);\n\
		gl_FragColor = vec4(gl_FragCoord.x/640.0, 0.0, gl_FragCoord.z, 1.0);\n\
}\n";

int main ()
{
	int ab=0, eab=0, i=0, iOffset=0;
	unsigned int position_attrib_idx = 0;
	unsigned int colour_attrib_idx = 1;
	float vOffset[3] = {-1.0f,-1.0f,-1.0f}; //change values

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
	iOffset = GetUniformLocation(&program, "offset");
	printf("%d",iOffset);
	
	ab = CreateBufferUsingData(BUFFER_ARRAY, BUFFER_STATIC, sizeof(vertices), vertices);
	eab = CreateBufferUsingData(BUFFER_ELEMENTS, BUFFER_STATIC, sizeof(indices), indices);

	//-------------------
	//tell openGL how to interperet the data
	BindBuffer(BUFFER_ELEMENTS, eab);
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
	while (!IS_BUTTON_PRESSED(platform.m_keyboard.key[KB_ESC]) && !platform.m_quit)
	{
		Tick(&platform);
		ClearScreenBuffer(COLOR_BIT);
		vOffset[0] = vOffset[1] = vOffset[2] = -1.0f;
		
		for(i=0;i<SQUARES;i++)
		{
			SetVec3ByLocation(iOffset, vOffset);
			Draw(DRAW_TRIANGLE_STRIP, ELEMENT_BYTE, sizeof(indices)/sizeof(unsigned char), 0);		
			vOffset[0] += STEP_RANGE/(SQUARES-1);
			vOffset[1] += STEP_RANGE/(SQUARES-1);
			vOffset[2] += 2.0f/(SQUARES);
		}
		SwapBuffers(&platform);		
	}
	Stop();	
	DeleteShaderProgram(&program);
	DeleteShader(&vertexShader);
	DeleteShader(&fragmentShader);
	Close(&platform);
}