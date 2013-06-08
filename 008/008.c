#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <platform.h>
#include <shader_gl.h>

#include <wrapper_gl.h>
#include <transforms.h>
#include <math.h>


#define SQUARE_HW 0.05f
static const float vertices[] = {
	 -SQUARE_HW,  -SQUARE_HW, 0.0f, //P
	 1.0f,  1.0f, 1.0f,				//C
	 //1
	 SQUARE_HW,  -SQUARE_HW, 0.0f,	//P
	 1.0f,  1.0f, 1.0f,				//C
	 //2
	 SQUARE_HW,  SQUARE_HW, 0.0f,	//P
	 1.0f,  1.0f, 1.0f,				//C
	 //3
	 -SQUARE_HW,  SQUARE_HW, 0.0f,	//P
	 1.0f,  1.0f, 1.0f,				//C
};
static unsigned char indices[] = {	0, 1, 3, 2 };

static const char* pVertexShader = "\
attribute vec4 position;\n\attribute vec3 colour;\n\uniform mat4 mt;\n\\n\varying vec3 vColour;\n\void main() {\n\	gl_Position = mt *  vec4(position.xyz,1.0);\n\	//gl_Position = position;\n\	vColour = colour;\n\}\n";

static const char* pFragmentShader = "\
varying vec3 vColour;\n\
void main() {\n\		gl_FragColor = vec4(vColour,1.0);\n\
}\n";


int main ()
{
	int ab=0, eab=0, uMtx=0, i=0;
	unsigned int position_attrib_idx = 0;
	unsigned int colour_attrib_idx = 1;

	//transform stuff
	STransform transform;

	//GL stuff
	SShader vertexShader;
	SShader fragmentShader;
	SShaderProgram program;

	CPlatform platform;
	Create(&platform, "", 2, 1, 640, 640, 8, 8, 8, 8, 16, 8, 0);	

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
	uMtx = GetUniformLocation(&program, "mt");

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
	//set rendering states
	EnableCulling(1);	

	SetClearColour(0, 0, 0, 0.0f); //alpha to 0, should make triangle appear over console
	
	Start(&program); 
	TransformInitialise(&transform, 10);	
	while (!IS_BUTTON_PRESSED(platform.m_keyboard.key[KB_ESC]))
	{
		Tick(&platform);
		ClearScreenBuffer(COLOR_BIT);		

		SetMtx44ByLocation(uMtx, TransformGetCurrentMatrix(&transform)->data);
		Draw(DRAW_TRIANGLE_STRIP, ELEMENT_BYTE, sizeof(indices)/sizeof(unsigned char), 0);

		TransformPush(&transform);
			TransformTranslate(&transform, 0.2, 0, 0);
			SetMtx44ByLocation(uMtx, TransformGetCurrentMatrix(&transform)->data);
			Draw(DRAW_TRIANGLE_STRIP, ELEMENT_BYTE, sizeof(indices)/sizeof(unsigned char), 0);

			TransformPush(&transform);
				TransformTranslate(&transform, 0, 0.2f, 0);				
				SetMtx44ByLocation(uMtx, TransformGetCurrentMatrix(&transform)->data);
				Draw(DRAW_TRIANGLE_STRIP, ELEMENT_BYTE, sizeof(indices)/sizeof(unsigned char), 0);
			TransformPop(&transform);
		TransformPop(&transform);

		TransformPush(&transform);
			TransformTranslate(&transform, -0.2f, -0.2f, 0);
			SetMtx44ByLocation(uMtx, TransformGetCurrentMatrix(&transform)->data);
			Draw(DRAW_TRIANGLE_STRIP, ELEMENT_BYTE, sizeof(indices)/sizeof(unsigned char), 0);
		TransformPop(&transform);
		SwapBuffers(&platform);		
	}
	Stop();	
	DeleteShaderProgram(&program);
	DeleteShader(&vertexShader);
	DeleteShader(&fragmentShader);
	Close(&platform);
}