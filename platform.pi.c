#include "platform.h"

#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <bcm_host.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <assert.h>
#include <fcntl.h>
#include <string.h>
#include <linux/input.h>
//#include "keymap.h"

typedef struct
{
	int kb_input;
	int mouse_input;
	uint32_t screen_width;
	uint32_t screen_height;
	// OpenGL|ES objects
	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	EGL_DISPMANX_WINDOW_T nativewindow;

} STATE;


static void PrintConfigAttributes(EGLDisplay display, EGLConfig config)
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

int Create(CPlatform* pPlatform, char* title, int glMajor, int glMinor,  int width, int height, int redBits, int greenBits, int blueBits, int alphaBits,int depthBits, int stencilBits, int nSamples)
{
	char name[256] = "Unknown";
	int32_t success = 0;
	EGLBoolean result;
	EGLint num_config;
	
	DISPMANX_ELEMENT_HANDLE_T	dispman_element;
	DISPMANX_DISPLAY_HANDLE_T	dispman_display;
	DISPMANX_UPDATE_HANDLE_T	dispman_update;
	//VC_DISPMANX_ALPHA_T			dispman_alpha;
	VC_RECT_T dst_rect;
	VC_RECT_T src_rect;
	EGLConfig config;
	STATE* pState = 0;

	pPlatform->m_size.x = width;
	pPlatform->m_size.y = height;

	EGLint * pegl = 0;
	EGLint attribute_list[] =
	{
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_DEPTH_SIZE, 16,
		EGL_STENCIL_SIZE, 8,
		EGL_SAMPLE_BUFFERS, 0,
		EGL_SAMPLES, 0,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_NONE
	};   

	EGLint gles_attribute_list[] =
	{
		EGL_CONTEXT_CLIENT_VERSION, 1,
		EGL_NONE
	};   	
	
	attribute_list[1] = redBits;
	attribute_list[3] = greenBits;
	attribute_list[5] = blueBits;
	attribute_list[7] = alphaBits;	
	attribute_list[9] = depthBits;	
	attribute_list[11] = stencilBits;	
	
	pegl  = attribute_list;

	if (nSamples > 0 )
	{
		attribute_list[13] = 1;	
		attribute_list[15] = nSamples;	
	}	
	
	gles_attribute_list[1] = glMajor;

	switch(eglQueryAPI())
	{
		case EGL_OPENGL_API:
			printf("EGL_OPENGL_API\n");break;
		case EGL_OPENGL_ES_API:
			printf("EGL_OPENGL_ES_API\n");break;
		case EGL_OPENVG_API:
			printf("EGL_OPENVG_API\n");break;
	}
	/*
	eglBindAPI(EGL_OPENVG_API);
	switch(eglQueryAPI())
	{
		case EGL_OPENGL_API:
			printf("EGL_OPENGL_API\n");break;
		case EGL_OPENGL_ES_API:
			printf("EGL_OPENGL_ES_API\n");break;
		case EGL_OPENVG_API:
			printf("EGL_OPENVG_API\n");break;
	}*/

	pState = (STATE*)malloc(sizeof(STATE));
	pPlatform->m_pData = (void*)pState;

	//--------------------------------
	//platform specific stuff
	//--------------------------------
	bcm_host_init();
	// create an EGL window surface
	success = graphics_get_display_size(0 /* LCD */, &pState->screen_width, &pState->screen_height);	
	assert( success >= 0 );

	dst_rect.x = 0;
	dst_rect.y = 0;
	dst_rect.width = pState->screen_width;	//resolution on the display device
	dst_rect.height = pState->screen_height;//ditto

	src_rect.x = 0;
	src_rect.y = 0;	
	src_rect.width = width << 16;	//gl rendered resolution
	src_rect.height = height << 16;	//ditto	

	dispman_display = vc_dispmanx_display_open( 0 /* LCD */);
	dispman_update = vc_dispmanx_update_start( 0 );

	//dispman_alpha.flags = DISPMANX_FLAGS_ALPHA_FIXED_ALL_PIXELS; 
	//dispman_alpha.opacity = 0x55; 
	//dispman_alpha.mask = NULL; 

	dispman_element = vc_dispmanx_element_add ( dispman_update, dispman_display,
		0/*layer*/, &dst_rect, 0/*src*/,
		&src_rect, DISPMANX_PROTECTION_NONE, 0/*alpha*/, 0/*clamp*/, 0/*transform*/);
		//&src_rect, DISPMANX_PROTECTION_NONE, &dispman_alpha /*alpha*/, 0/*clamp*/, 0/*transform*/);

	pState->nativewindow.element = dispman_element;
	pState->nativewindow.width = width;			//should be teh same as the src resolution before the shift
	pState->nativewindow.height = height;		//ditto
	vc_dispmanx_update_submit_sync( dispman_update );

	//--------------------------------
	//end platform specific stuff
	//--------------------------------

	// get an EGL display connection
	pState->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	assert(pState->display!=EGL_NO_DISPLAY);

	// initialize the EGL display connection
	result = eglInitialize(pState->display, NULL, NULL);
	assert(EGL_FALSE != result);	

	// get an appropriate EGL frame buffer configuration
	//http://www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglChooseConfig.html
	result = eglChooseConfig(pState->display, attribute_list, &config, 1, &num_config);
	assert(EGL_FALSE != result);

	// create an EGL rendering context
	pState->context = eglCreateContext(pState->display, config, EGL_NO_CONTEXT, gles_attribute_list);
	assert(pState->context!=EGL_NO_CONTEXT);

	pState->surface = eglCreateWindowSurface( pState->display, config, &pState->nativewindow, NULL );
	assert(pState->surface != EGL_NO_SURFACE);

	// connect the context to the surface
	result = eglMakeCurrent(pState->display, pState->surface, pState->surface, pState->context);
	assert(EGL_FALSE != result);

	PrintConfigAttributes(pState->display, config);

	//setup keyboard and mouse input
	memset(pPlatform->m_keyboard.key, 0, KB_MAX);
	pState->kb_input = open("/dev/input/event1", O_RDONLY|O_NONBLOCK);
	ioctl (pState->kb_input, EVIOCGNAME (sizeof (name)), name);
	printf ("keyboard: %s\n", name);
}

#define MAX_INPUT_EVENTS 20
void Tick(CPlatform* pPlatform)
{
	char tmp = 0;
	int rd = 0, idx = 0, km_idx;
	struct input_event ie[MAX_INPUT_EVENTS];
	int size = sizeof(struct input_event);
	STATE* pState = (STATE*)pPlatform->m_pData;

	//quickly go through and untoggle all the old pressed
	for(rd=0;rd<KB_MAX;rd++)
		BUTTON_UNTOGGLE(pPlatform->m_keyboard.key[rd]);

	rd = read (pState->kb_input, ie, size * MAX_INPUT_EVENTS);
	if( rd > 0)
	{
		while(rd>0)
		{
			if(ie[idx].type == EV_KEY && ie[idx].code < LINUX_KB_MAX)
			{
				km_idx = linux_to_keymap[ie[idx].code]; //get the keymap index
				tmp = pPlatform->m_keyboard.key[km_idx];
				BUTTON_RESET(pPlatform->m_keyboard.key[km_idx]);
				if(ie[idx].value)
				{
					BUTTON_PRESS(pPlatform->m_keyboard.key[km_idx]);
					if(!IS_BUTTON_PRESSED(tmp))
						BUTTON_TOGGLE(pPlatform->m_keyboard.key[km_idx]);
				}
				else
				{
					if(IS_BUTTON_PRESSED(tmp))
						BUTTON_TOGGLE(pPlatform->m_keyboard.key[km_idx]);
				}				
			}
			rd -= size;
			idx++;
		}
	}
}

void SwapBuffers(CPlatform* pPlatform)
{
	STATE* pState = (STATE*)pPlatform->m_pData;
	eglSwapBuffers(pState->display, pState->surface);
}

void Close(CPlatform* pPlatform)
{
	STATE* pState = (STATE*)pPlatform->m_pData;

	//close input streams
	close(pState->kb_input);

	eglSwapBuffers(pState->display, pState->surface);
	eglMakeCurrent( pState->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
	eglDestroySurface( pState->display, pState->surface );
	eglDestroyContext( pState->display, pState->context );
	eglTerminate( pState->display );
	free(pState);
}