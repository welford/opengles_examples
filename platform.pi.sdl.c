#include "platform.h"

#if RPI_PLATFORM_BOOKWORM_SDL

#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <fcntl.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

static SDL_Window* window = 0;
static SDL_GLContext glContext;
int Create(CPlatform* pPlatform, char* title, int glMajor, int glMinor,  int width, int height, int redBits, int greenBits, int blueBits, int alphaBits,int depthBits, int stencilBits, int nSamples)
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	// Set OpenGL ES version (e.g., OpenGL ES 2.0)
	//SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, glMajor);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, glMinor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, glMajor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, glMinor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

	// Create an SDL window
	window = SDL_CreateWindow("OpenGL ES Window",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			width, // width
			height, // height
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	// Create an OpenGL context associated with the window.
	glContext = SDL_GL_CreateContext(window);
	if (!glContext) {
		SDL_Log("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -1;
	}
	pPlatform->m_quit = 0;
	memset(&pPlatform->m_keyboard,0,sizeof(Keyboard));
	return 0;
}

#define MAX_INPUT_EVENTS 20
void Tick(CPlatform* pPlatform) {
	int rd;
	//quickly go through and untoggle all the old pressed
	for(rd=0;rd<KB_MAX;rd++)
		BUTTON_UNTOGGLE(pPlatform->m_keyboard.key[rd]);

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			pPlatform->m_quit = 1;
		}
		if (event.type == SDL_KEYDOWN && event.key.keysym.scancode < SDL_KB_MAX) {
			int km_idx = sdl_to_keymap[event.key.keysym.scancode]; //get the keymap index
			char tmp = pPlatform->m_keyboard.key[km_idx];
			BUTTON_RESET(pPlatform->m_keyboard.key[km_idx]);
			BUTTON_PRESS(pPlatform->m_keyboard.key[km_idx]);
			if(!IS_BUTTON_PRESSED(tmp))
				BUTTON_TOGGLE(pPlatform->m_keyboard.key[km_idx]);
		}
		if (event.type == SDL_KEYUP && event.key.keysym.scancode < SDL_KB_MAX) {
			int km_idx = sdl_to_keymap[event.key.keysym.scancode]; //get the keymap index
			char tmp = pPlatform->m_keyboard.key[km_idx];
			BUTTON_RESET(pPlatform->m_keyboard.key[km_idx]);
			if(IS_BUTTON_PRESSED(tmp))
				BUTTON_TOGGLE(pPlatform->m_keyboard.key[km_idx]);
		}
	}
}

void SwapBuffers(CPlatform* pPlatform)
{
	SDL_GL_SwapWindow(window);
}

void Close(CPlatform* pPlatform)
{
	// Clean up
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

#endif //RPI_ORIGINAL_PLATFORM