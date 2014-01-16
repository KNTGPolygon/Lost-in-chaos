#include <iostream>
#include <ctime>

#include "Utility.h"
#include "Window.h"
#include "Game.h"
#include "Resources.h"

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

	game.loadMap("dm_lockdown");

	game.time.delta = 1.0/(double)targetFps;

	long long fpst = clock()+1000;
	int fps = 0, fpsc = 0;

	long long t = clock(), dt = 0, tb = 0, sc = 0;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable (GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	
	resources.init ();
	resources.loadBunchOfTextures ();
	
	while (!glfwWindowShouldClose(window.handle) || window.key[27]==2)
	{
		game.time.speed = (window.key[' ']>0 ? 0.125 : 1.0);
		game.update();

		int w, h;
		double aspect;

		glfwGetWindowSize(window.handle,&w,&h);
		
		aspect = (double)w/(double)h;

		glViewport(0,0,w,h);
		glClearColor(0,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glLoadIdentity();

		glColor4f(1,1,1,1);
		resources.drawBackgroundTexture (0,game.camera.x/128.0);

		glOrtho(-aspect*8.0,aspect*8.0,-8,8,-1,1);
		glTranslated(-game.camera.x,-game.camera.y,0);
		
		game.draw();

		glFlush();

		WindowUpdate();

		fpsc++;
		if(clock()>=fpst)
		{
			fpst = clock()+1000;
			fps = fpsc;
			fpsc = 0;
		}

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

	WindowRelease();
    return 0;
}
