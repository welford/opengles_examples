#ifndef _PLATFORM_H_
#define _PLATFORM_H_

//#define RPI_PLATFORM_BOOKWORM_SDL 1
//#define RPI_PLATFORM_ORIGINAL 0 

#include "keymap.h"
#if RPI_PLATFORM_BOOKWORM_SDL
#include <SDL2/SDL_opengl.h>
#endif // RPI_PLATFORM_BOOKWORM_SDL

typedef struct 
{
	float x, y;
}PVec2f;

typedef struct 
{
	int x, y;
}PVec2i;

//Basic input states that we can expect from most 
//application instances
typedef struct
{
	int px,py;
	int dx,dy;
	int wheelRot;
	int lmb;
	int mmb;
	int rmb;
}SMouse;

#define BUTTON_PRESSED			0x1
#define BUTTON_TOGGLED			0x2
#define BUTTON_PRESS_TOGGLE		0x3
#define BUTTON_PRESS_RELEASE	0x2

#define BUTTON_RESET(A) A=0
#define BUTTON_PRESS(A) A|=BUTTON_PRESSED
#define BUTTON_TOGGLE(A) A|=BUTTON_TOGGLED
#define BUTTON_UNTOGGLE(A) A &=~BUTTON_TOGGLED

#define IS_BUTTON_PRESSED(A) (A & BUTTON_PRESSED)
#define IS_BUTTON_TOGGLE_PRESSED(A) (A == BUTTON_PRESS_TOGGLE)
#define IS_BUTTON_TOGGLE_RELEASED(A) (A == BUTTON_PRESS_RELEASE)

typedef struct
{
	char key[KB_MAX];
}Keyboard;

typedef struct
{
	PVec2f leftStick, rightStick;
	float leftTrigger,rightTrigger;
	char d_up,d_down,d_left,d_right; //dpad
	char fb_up,fb_down,fb_left,fb_right; //face buttons
	char start,select;
}Gamepad;

//not too keen on this, should just switch it to being pure C
//and maybe make a wrapper for it
typedef struct 
{
	int			m_quit;			//
	float		m_dt;			//delta time value
	PVec2i		m_size;			//
	PVec2i		m_position;		//
	SMouse		m_mouse;		//
	Keyboard	m_keyboard;		//
	Gamepad		m_gamepad;		//
	int			m_showingDebugConsole;
	void		*m_pData;		// platform specific data		
	
}CPlatform;

//allocate memory ebfore this
extern int Create(CPlatform* pPlatform, char* title, int glMajor, int glMinor, int width, int height, int redBits, int greeenBits, int blueBits, int alphaBits,int depthBits, int stencilBits, int nSamples);
extern void Tick(CPlatform* pPlatform);
extern void SwapBuffers(CPlatform* pPlatform);
extern void Close(CPlatform* pPlatform);

#endif //_PLATFORM_H_