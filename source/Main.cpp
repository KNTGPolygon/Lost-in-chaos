#include <iostream>

#include "Utility.h"
#include "Window.h"

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

	cout << "cout macka\n";
	double angle = 0.0;

	while (!glfwWindowShouldClose(window.handle) || window.key[27]==2)
	{
		int w, h;
		double aspect;

		glfwGetWindowSize(window.handle,&w,&h);
		
		aspect = (double)w/(double)h;

		glViewport(0,0,w,h);
		glClearColor(0,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT);

		glLoadIdentity();
		glOrtho(-aspect,aspect,-1,1,-1,1);
		glRotated(angle,0,0,1);

		glColor4f(1,1,1,1);
		glBegin(GL_TRIANGLES);
		glVertex2d(0.125,0.0);
		glVertex2d(-0.125,0.125);
		glVertex2d(-0.125,-0.125);
		glEnd();

		glFlush();

		angle+=1.0;

		WindowUpdate();
	}

	WindowRelease();
    return 0;
}
