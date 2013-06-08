#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bcm_host.h"

#include "GLES2/gl2.h"
#include "platform.h"

int main ()
{
	CPlatform platform;
	Create(&platform, "", 2, 1, 640, 480, 8, 8, 8, 8, 0, 0, 0);	

	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	while (!IS_BUTTON_PRESSED(platform.m_keyboard.key[KB_ESC]))
	{
		Tick(&platform);
		glClear( GL_COLOR_BUFFER_BIT );
		SwapBuffers(&platform);
	}
	glClear( GL_COLOR_BUFFER_BIT );
	Close(&platform);
}