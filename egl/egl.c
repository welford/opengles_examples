#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "EGL/egl.h"
#include "EGL/eglext.h"

void PrintConfigAttributes(EGLDisplay display, EGLConfig config)
{	
	EGLBoolean result;
	EGLint value;
	printf("--------------------------------------------------------------------------\n",value);
	eglGetConfigAttrib(display,config,EGL_CONFIG_ID,&value);
	printf("EGL_CONFIG_ID %d\n",value);		

	eglGetConfigAttrib(display,config,EGL_BUFFER_SIZE,&value);
	printf("EGL_BUFFER_SIZE %d\n",value);
	eglGetConfigAttrib(display,config,EGL_RED_SIZE,&value);
	printf("EGL_RED_SIZE %d\n",value);
	eglGetConfigAttrib(display,config,EGL_GREEN_SIZE,&value);
	printf("EGL_GREEN_SIZE %d\n",value);
	eglGetConfigAttrib(display,config,EGL_BLUE_SIZE,&value);
	printf("EGL_BLUE_SIZE %d\n",value);
	eglGetConfigAttrib(display,config,EGL_ALPHA_SIZE,&value);
	printf("EGL_ALPHA_SIZE %d\n",value);
	eglGetConfigAttrib(display,config,EGL_DEPTH_SIZE,&value);
	printf("EGL_DEPTH_SIZE %d\n",value);
	eglGetConfigAttrib(display,config,EGL_STENCIL_SIZE,&value);
	printf("EGL_STENCIL_SIZE %d\n",value);
	eglGetConfigAttrib(display,config,EGL_SAMPLE_BUFFERS,&value);
	printf("EGL_SAMPLE_BUFFERS %d\n",value);
	eglGetConfigAttrib(display,config,EGL_SAMPLES,&value);
	printf("EGL_SAMPLES %d\n",value);

	eglGetConfigAttrib(display,config,EGL_CONFIG_CAVEAT,&value);
	switch(value)
	{
		case  EGL_NONE : printf("EGL_CONFIG_CAVEAT EGL_NONE\n"); break;
		case  EGL_SLOW_CONFIG : printf("EGL_CONFIG_CAVEAT EGL_SLOW_CONFIG\n"); break;
	}	

	eglGetConfigAttrib(display,config,EGL_MAX_PBUFFER_WIDTH,&value);
	printf("EGL_MAX_PBUFFER_WIDTH %d\n",value);
	eglGetConfigAttrib(display,config,EGL_MAX_PBUFFER_HEIGHT,&value);
	printf("EGL_MAX_PBUFFER_HEIGHT %d\n",value);
	eglGetConfigAttrib(display,config,EGL_MAX_PBUFFER_PIXELS,&value);
	printf("EGL_MAX_PBUFFER_PIXELS %d\n",value);
	eglGetConfigAttrib(display,config,EGL_NATIVE_RENDERABLE,&value);
	printf("EGL_NATIVE_RENDERABLE %s \n",(value ? "true" : "false"));
	eglGetConfigAttrib(display,config,EGL_NATIVE_VISUAL_ID,&value);
	printf("EGL_NATIVE_VISUAL_ID %d\n",value);
	eglGetConfigAttrib(display,config,EGL_NATIVE_VISUAL_TYPE,&value);
	printf("EGL_NATIVE_VISUAL_TYPE %d\n",value);				
	eglGetConfigAttrib(display,config,EGL_SURFACE_TYPE,&value);
	printf("EGL_SURFACE_TYPE %d\n",value);
	eglGetConfigAttrib(display,config,EGL_TRANSPARENT_TYPE,&value);
	printf("EGL_TRANSPARENT_TYPE %d\n",value);	
}

int main ()
{
	int mjr, mnr, i;
	EGLDisplay display;	
	EGLBoolean result;
	EGLint numConfigs;
	EGLConfig *configs = 0;	

	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	assert(display !=EGL_NO_DISPLAY);
	
	result = eglInitialize(display, &mjr, &mnr);
	assert(result != EGL_FALSE );
	printf("EGL v%d.%d\n",mjr, mnr);
	
	result=eglGetConfigs(display,NULL,0,&numConfigs);
	assert(result != EGL_FALSE );
	printf( "number of configs found %d\n", numConfigs);
	// now we create a buffer to store all our configs
	configs = (EGLConfig*)malloc(sizeof(EGLConfig)*numConfigs);

	// and copy them into our buffer (don't forget to delete once done)
	result=eglGetConfigs(display,configs,numConfigs,&numConfigs);
	assert(result != EGL_FALSE );

	for(i=0; i<numConfigs; ++i)
		PrintConfigAttributes(display, configs[i]);
	//clean up
	free(configs);
	printf("done\n");
}



