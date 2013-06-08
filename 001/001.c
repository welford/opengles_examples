#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bcm_host.h"

#include <GLES2/gl2.h>

#include "platform.h"

int main ()
{
	int i=0;
	float clr[3] = {0.0f, 0.3f, 0.6f};
	float add[3] = {0.001f, 0.002f, 0.003f};

	bcm_host_init();
	CPlatform platform;
	Create(&platform, "", 2, 1, 640, 480, 5, 6, 5, 0, 24, 8, 0);	

	while (!IS_BUTTON_PRESSED(platform.m_keyboard.key[KB_ESC]))
	{
		Tick(&platform);
		glClearColor(clr[0], clr[1], clr[2], 1.0f);
		glClear( GL_COLOR_BUFFER_BIT );
		SwapBuffers(&platform);

		for(i=0;i<3;i++)
		{
			clr[i]+=add[i];
			if(clr[i]>1.0f)add[i]*=-1.0f;		
			if(clr[i]<0.0f)add[i]*=-1.0f;
		}
	}
	glClear( GL_COLOR_BUFFER_BIT );
	Close(&platform);
}