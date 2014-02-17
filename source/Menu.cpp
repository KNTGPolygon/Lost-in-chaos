#include "Menu.h"
#include "Game.h"
#include "Resources.h"

using namespace std;

Menu menu;

void TW_CALL EventNewGame(void*)
{
	TwDefine(" MainMenu visible=false ");
	game.loadMap("dm_lockdown");
	menu.mode = MENU_GAME;
}

void TW_CALL EventExit(void*)
{
	menu.mode = MENU_EXIT;
}

void TW_CALL EventBack(void*)
{
	TwDefine(" GameMenu visible=false ");
	TwDefine(" MainMenu visible=true ");
	game.clear();
	menu.mode = MENU_MAIN;
}

void MenuInit()
{
	menu.mode = MENU_INTRO;

	menu.main = TwNewBar("MainMenu");
	menu.game = TwNewBar("GameMenu");

	TwDefine(" TW_HELP visible=false ");
	TwDefine(" MainMenu visible=false ");
	TwDefine(" GameMenu visible=false ");

	TwAddButton(menu.main,"newgame",EventNewGame,0," label='New game' ");
	TwAddButton(menu.main,"exit",EventExit,0," label='Exit' ");

	TwAddButton(menu.game,"back",EventBack,0," label='Back to main menu' ");

	menu.introTime = 0;
	menu.menuTime = 0;
}

inline double sigma(double x)
{
	return 1/(1+exp(-x));
}

void MenuUpdate()
{
	double f;
	switch(menu.mode)
	{
		case MENU_INTRO:
		glLoadIdentity();
		f = sin(PI*menu.introTime/INTRO_LENGTH);
		glColor4f(1,1,1,f*f);
		resources.drawBackgroundTexture(31,0);
		if(window.key[GLFW_KEY_ESCAPE]==2)
			menu.introTime = INTRO_LENGTH;
		menu.introTime+=1.0/60.0;
		if(menu.introTime>=INTRO_LENGTH)
		{
			menu.mode = MENU_MAIN;
			TwDefine(" MainMenu visible=true ");
		}
		break;
		case MENU_MAIN:
		TwDraw();
		glBindTexture(GL_TEXTURE_2D,0);
		glColor4f(0,0,0,sigma(-2*menu.menuTime+6));
		glBegin(GL_QUADS);
		glVertex2f(-1,-1);
		glVertex2f(-1,1);
		glVertex2f(1,1);
		glVertex2f(1,-1);
		glEnd();
		menu.menuTime+=1.0/60.0;
		break;
		case MENU_GAME:
		if(window.key[GLFW_KEY_ESCAPE]==2)
		{
			menu.visible = !menu.visible;
			if(menu.visible)
				TwDefine(" GameMenu visible=true ");
			else
				TwDefine(" GameMenu visible=false ");
		}
		game.time.speed = (window.key[' ']>0 ? 0.125 : 1.0);
		game.update();
		game.draw();
		TwDraw();
		break;
	}
}

void MenuRelease()
{
}
