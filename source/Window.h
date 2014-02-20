#pragma once

#include <map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <AntTweakBar.h>

#define WINDOW_MODE_NONE 0
#define WINDOW_MODE_WINDOWED 1
#define WINDOW_MODE_FULLSCREEN 2

struct Window
{
	GLFWwindow *hidden, *handle;
	int mode, width, height, lastW, lastH, mouseMode;
	double aspect;

	double mouseX, mouseY, scrollX, scrollY;
	
	int key[512];

	std::map<int,int> keyCodeGLFW;
};

extern Window window;

void WindowUpdate();

int WindowCreate(int width, int height);
int WindowCreateFullscreen();
void WindowDestroy();

int WindowInitialize();
void WindowRelease();
