#include <iostream>
#include <ctime>

#include "Utility.h"
#include "Window.h"
#include "Game.h"
#include "Resources.h"
#include "Menu.h"

#include <Windows.h>

using namespace std;

const long long BASE = 2*3*4*5*6*7*8*9*10;

long long targetFps = 60;

int main(int argc, char* argv[])
{
	cout << "I'm lost in chaos with " << (8*sizeof(void*)) << " bits.\n";

	if(WindowInitialize()!=0)
		return 0;

	if(WindowCreate(640,480)!=0)
	{
		WindowRelease();
		return 0;
	}

	glfwSwapInterval(0);

	#ifdef _WIN32
	typedef BOOL (WINAPI *PFNWGLSWAPINTERVALEXTPROC)(int interval);
	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if(wglSwapIntervalEXT)
		wglSwapIntervalEXT(0);
	#endif

	cout << "*** sterowanie: WSAD, SPACJA(slowmo), F(fps), Esc(wyjscie) ***\n";

	game.time.delta = 1.0/(double)targetFps;

	long long fpst = clock()+1000;
	int fps = 0, fpsc = 0;

	long long t = clock(), dt = 0, tb = 0, sc = 0;
	
	int err = resources.init();
	if(err!=0)
	{
		cout << "resources error " << err << "\n";
		return 0;
	}
	resources.load ();

	MenuInit();

	ogg_stream music;

	music.open("music/Menu.ogg");
	music.playback();
	
	while (!glfwWindowShouldClose(window.handle) && menu.mode!=MENU_EXIT)
	{
		glViewport(0,0,window.width,window.height);
		glClearColor(0,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT);
		
		MenuUpdate();

		glFlush();

		WindowUpdate();

		fpsc++;
		if(clock()>=fpst)
		{
			fpst = clock()+1000;
			fps = fpsc;
			fpsc = 0;
		}

		music.update();

		if(window.key['F']==2)
			cout << "fps = " << fps << "\n";

		dt = clock()-t;
		t+=dt;

		tb += max(BASE*1000ll/targetFps-BASE*(dt-sc),0ll);
		if(tb>=BASE)
		{
			sc = tb/BASE;
			Sleep(sc);
			tb%=BASE;
		}
		else
			sc = 0;
	}

	music.release();

	resources.release();
	MenuRelease();
	WindowRelease();

    return 0;
}
