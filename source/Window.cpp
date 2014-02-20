#include <iostream>
#include "Window.h"
#include "Utility.h"
#include <AntTweakBar.h>
#include <Windows.h>
#include "Draw.h"

#define VSYNC 0

using namespace std;

Window window;

void glfw_error_callback(int error, const char* description)
{
	SAFECOUT("GLFW error "<<error<<": "<<description<<"\n");
	//ThEngineExit();
}

void glfw_window_size_callback(GLFWwindow *pwnd, int w, int h)
{
	if(window.mode==WINDOW_MODE_WINDOWED)
		window.lastW = w, window.lastH = h;
	if(window.width!=w || window.height!=h)
	{
		DrawDestroyBuffers();
		DrawCreateBuffers(w,h);
		window.width = w;
		window.height = h;
	}
	TwWindowSize(window.width,window.height);
}

void glfw_window_key_callback(GLFWwindow *pwnd, int key, int scancode, int action, int mods)
{
	TwEventKeyGLFW(window.keyCodeGLFW[key],action);
	switch(action)
	{
		case GLFW_PRESS:
		window.key[key] = 2;
		break;
		case GLFW_RELEASE:
		window.key[key] = -1;
		break;
		case GLFW_REPEAT:
		window.key[key] = 3;
		break;
	}
}

void glfw_mouse_button_callback(GLFWwindow *pwnd, int button, int action, int mods)
{
	TwEventMouseButtonGLFW(button,action);
	if(action==GLFW_PRESS)
		window.key[500+button] = 2;
	else
		window.key[500+button] = -1;
}

void glfw_scroll_callback(GLFWwindow *pwnd, double xoffset, double yoffset)
{
	window.scrollX = xoffset;
	window.scrollY = yoffset;
}

void glfw_char_callback(GLFWwindow *pwnd, unsigned int input)
{
}

void WindowSetMouseMode(int mode)
{
	if(window.mouseMode==mode)
		return;
	switch(mode)
	{
		case 0:
		glfwSetInputMode(window.handle,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
		glfwSetCursorPos(window.handle,window.mouseX,window.mouseY);
		break;
		case 1:
		glfwSetInputMode(window.handle,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
		glfwSetCursorPos(window.handle,window.mouseX,window.mouseY);
		break;
		case 2:
		glfwSetInputMode(window.handle,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
		glfwGetCursorPos(window.handle,&window.mouseX,&window.mouseY);
		glfwSetCursorPos(window.handle,0.0,0.0);
		break;
	}
	window.mouseMode = mode;
}

void WindowUpdate()
{
	for(int i=0;i<512;i++)
		window.key[i] = (window.key[i]>0 ? 1 : 0);
	glfwPollEvents();

	if(window.key[GLFW_KEY_F11]==2)
	{
		int mm = window.mouseMode;
		WindowSetMouseMode(0);
		WindowDestroy();
		if(window.mode==WINDOW_MODE_WINDOWED)
			WindowCreateFullscreen();
		else
			WindowCreate(window.lastW,window.lastH);
		WindowSetMouseMode(mm);
	}

	{
		double mx, my;
		glfwGetCursorPos(window.handle,&mx,&my);
		TwEventMousePosGLFW(mx,my);
	}

	glfwSwapBuffers(window.handle);
}

void WindowSetUp()
{
	glfwSetWindowSizeCallback(window.handle,glfw_window_size_callback);
	glfwSetKeyCallback(window.handle,glfw_window_key_callback);
	glfwSetMouseButtonCallback(window.handle,glfw_mouse_button_callback);
	glfwSetScrollCallback(window.handle,glfw_scroll_callback);
	glfwSetCharCallback(window.handle,glfw_char_callback);

	glfwMakeContextCurrent(window.handle);

	glfwSwapInterval(VSYNC);

	#ifdef _WIN32
	typedef BOOL (WINAPI *PFNWGLSWAPINTERVALEXTPROC)(int interval);
	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if(wglSwapIntervalEXT)
		wglSwapIntervalEXT(VSYNC);
	#endif

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable (GL_TEXTURE_2D);
	glActiveTexture( GL_TEXTURE0 );
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glActiveTexture( GL_TEXTURE1 );
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glActiveTexture( GL_TEXTURE0 );
}

int WindowCreate(int w, int h)
{
	if(window.mode!=WINDOW_MODE_NONE)
		return -1;

	window.lastW = window.width = w, window.lastH = window.height = h;
	window.handle = glfwCreateWindow(window.width,window.height,"Lost in chaos",0,window.hidden);
	TwWindowSize(window.width,window.height);

	if(window.handle==0)
		return -2;

	WindowSetUp();

	if(DrawCreateBuffers(window.width,window.height)!=0)
		return -3;

	window.mode = WINDOW_MODE_WINDOWED;
	return 0;
}

int WindowCreateFullscreen()
{
	const GLFWvidmode *vm = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int r = 0;

	window.width = vm->width, window.height = vm->height;
	window.handle = glfwCreateWindow(window.width,window.height,"Lost in chaos",glfwGetPrimaryMonitor(),window.hidden);

	TwWindowSize(window.width,window.height);

	if(window.handle==0)
		return -1;
	else
	{
		r = GL_TRUE;
		WindowSetUp();
		//glfwWindowHint(GLFW_REFRESH_RATE,engine.frequency);
		window.mode = WINDOW_MODE_FULLSCREEN;
	}

	if(DrawCreateBuffers(window.width,window.height)!=0)
		return -2;

	return 0;
}

void WindowDestroy()
{
	if(window.handle==0)
		return;
	DrawDestroyBuffers();
	glfwDestroyWindow(window.handle);
	window.handle = 0;
}

int WindowInitialize()
{
	if (glfwInit()!=GL_TRUE)
	{
		SAFECOUT("GLFW error\n");
		return -1;
	}
	window.mode = WINDOW_MODE_NONE;
	window.hidden = glfwCreateWindow(32,24,"The Hidden One",0,0);
	glfwHideWindow(window.hidden);
	if(window.hidden==0)
	{
		SAFECOUT("Hidden window error\n");
		glfwTerminate();
		return -2;
	}
	glfwMakeContextCurrent(window.hidden);
	if(glewInit()!=GLEW_OK)
	{
		SAFECOUT("GLEW error");
		glfwTerminate();
		return -3;
	}
	window.mouseMode = 0;
	TwInit(TW_OPENGL, NULL);
	for(int i=0;i<349;i++)
		window.keyCodeGLFW[i] = i;
	window.keyCodeGLFW[GLFW_KEY_ESCAPE] = 256+1;
	for(int i=0;i<25;i++)
		window.keyCodeGLFW[GLFW_KEY_F1+i] = 256+2+i;
	window.keyCodeGLFW[GLFW_KEY_UP] = 256+27;
	window.keyCodeGLFW[GLFW_KEY_DOWN] = 256+28;
	window.keyCodeGLFW[GLFW_KEY_LEFT] = 256+29;
	window.keyCodeGLFW[GLFW_KEY_RIGHT] = 256+30;
	window.keyCodeGLFW[GLFW_KEY_LEFT_SHIFT] = (256+31);
	window.keyCodeGLFW[GLFW_KEY_RIGHT_SHIFT] = (256+32);
	window.keyCodeGLFW[GLFW_KEY_LEFT_CONTROL      ] = (256+33);
	window.keyCodeGLFW[GLFW_KEY_RIGHT_CONTROL      ] = (256+34);
	window.keyCodeGLFW[GLFW_KEY_LEFT_ALT       ] = (256+35);
	window.keyCodeGLFW[GLFW_KEY_RIGHT_ALT       ] = (256+36);
	window.keyCodeGLFW[GLFW_KEY_TAB        ] = (256+37);
	window.keyCodeGLFW[GLFW_KEY_ENTER      ] = (256+38);
	window.keyCodeGLFW[GLFW_KEY_BACKSPACE  ] = (256+39);
	window.keyCodeGLFW[GLFW_KEY_INSERT     ] = (256+40);
	window.keyCodeGLFW[GLFW_KEY_DELETE        ] = (256+41);
	window.keyCodeGLFW[GLFW_KEY_PAGE_UP     ] = (256+42);
	window.keyCodeGLFW[GLFW_KEY_PAGE_DOWN   ] = (256+43);
	window.keyCodeGLFW[GLFW_KEY_HOME       ] = (256+44);
	window.keyCodeGLFW[GLFW_KEY_END        ] = (256+45);
	window.keyCodeGLFW[GLFW_KEY_KP_0       ] = (256+46);
	window.keyCodeGLFW[GLFW_KEY_KP_1       ] = (256+47);
	window.keyCodeGLFW[GLFW_KEY_KP_2       ] = (256+48);
	window.keyCodeGLFW[GLFW_KEY_KP_3       ] = (256+49);
	window.keyCodeGLFW[GLFW_KEY_KP_4       ] = (256+50);
	window.keyCodeGLFW[GLFW_KEY_KP_5       ] = (256+51);
	window.keyCodeGLFW[GLFW_KEY_KP_6       ] = (256+52);
	window.keyCodeGLFW[GLFW_KEY_KP_7       ] = (256+53);
	window.keyCodeGLFW[GLFW_KEY_KP_8       ] = (256+54);
	window.keyCodeGLFW[GLFW_KEY_KP_9       ] = (256+55);
	window.keyCodeGLFW[GLFW_KEY_KP_DIVIDE  ] = (256+56);
	window.keyCodeGLFW[GLFW_KEY_KP_MULTIPLY] = (256+57);
	window.keyCodeGLFW[GLFW_KEY_KP_SUBTRACT] = (256+58);
	window.keyCodeGLFW[GLFW_KEY_KP_ADD     ] = (256+59);
	window.keyCodeGLFW[GLFW_KEY_KP_DECIMAL ] = (256+60);
	window.keyCodeGLFW[GLFW_KEY_KP_EQUAL   ] = (256+61);
	window.keyCodeGLFW[GLFW_KEY_KP_ENTER   ] = (256+62);
	window.keyCodeGLFW[GLFW_KEY_NUM_LOCK] = (256+63);
	window.keyCodeGLFW[GLFW_KEY_CAPS_LOCK  ] = (256+64);
	window.keyCodeGLFW[GLFW_KEY_SCROLL_LOCK] = (256+65);
	window.keyCodeGLFW[GLFW_KEY_PAUSE      ] = (256+66);
	window.keyCodeGLFW[GLFW_KEY_LEFT_SUPER     ] = (256+67);
	window.keyCodeGLFW[GLFW_KEY_RIGHT_SUPER     ] = (256+68);
	window.keyCodeGLFW[GLFW_KEY_MENU       ] = (256+69);
	return 0;
}

void WindowRelease()
{
	TwTerminate();
	glfwTerminate();
}
