#pragma once

#include "Window.h"

#define MENU_EXIT -1
#define MENU_INTRO 0
#define MENU_MAIN 1
#define MENU_GAME 2

#define INTRO_LENGTH 5.0

struct Menu
{
	int mode;
	bool visible;
	TwBar *main, *game;
	double introTime, menuTime;
};

extern Menu menu;

void MenuInit();
void MenuUpdate();
void MenuRelease();
