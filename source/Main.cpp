#include <iostream>
#include <ctime>

#include "Utility.h"
#include "Window.h"
#include "Game.h"

using namespace std;

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

	glfwSwapInterval(1);

	cout << "*** sterowanie: WSAD ***\n";

	game.loadMap("map/dm_lockdown.txt");

	long long fpst = clock()+1000;
	int fps = 0, fpsc = 0;

	while (!glfwWindowShouldClose(window.handle) || window.key[27]==2)
	{
		game.update();

		int w, h;
		double aspect;

		glfwGetWindowSize(window.handle,&w,&h);
		
		aspect = (double)w/(double)h;

		glViewport(0,0,w,h);
		glClearColor(0,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT);

		glLoadIdentity();
		glOrtho(-aspect*8.0,aspect*8.0,-8,8,-1,1);
		
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
	}

	WindowRelease();
    return 0;
}
