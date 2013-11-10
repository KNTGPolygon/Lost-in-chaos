#include "Window.h"
#include "Utility.h"

Window window;

void glfw_error_callback(int error, const char* description)
{
	SAFECOUT("GLFW error "<<error<<": "<<description<<"\n");
	//ThEngineExit();
}

void glfw_window_size_callback(GLFWwindow *pwnd, int w, int h)
{
	window.width = w;
	window.height = h;
}

void glfw_window_key_callback(GLFWwindow *pwnd, int key, int scancode, int action, int mods)
{
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

void WindowUpdate()
{
	for(int i=0;i<512;i++)
		window.key[i] = (window.key[i]>0 ? 1 : 0);
	glfwSwapBuffers(window.handle);
	glfwPollEvents();
}

void ThWindowSetUp()
{
	glfwSetWindowSizeCallback(window.handle,glfw_window_size_callback);
	glfwSetKeyCallback(window.handle,glfw_window_key_callback);
	glfwSetMouseButtonCallback(window.handle,glfw_mouse_button_callback);
	glfwSetScrollCallback(window.handle,glfw_scroll_callback);
	glfwSetCharCallback(window.handle,glfw_char_callback);
	glfwMakeContextCurrent(window.handle);
	glfwSwapInterval(1);
}

int WindowCreate(int w, int h)
{
	if(window.mode!=WINDOW_MODE_NONE)
		return -1;

	window.width = w, window.height = h;
	window.handle = glfwCreateWindow(window.width,window.height,"Lost in chaos",0,window.hidden);

	if(window.handle==0)
		return -2;

	ThWindowSetUp();

	window.mode = WINDOW_MODE_WINDOWED;
	return 0;
}

void WindowDestroy()
{
	if(window.handle==0)
		return;
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
	return 0;
}

void WindowRelease()
{
	glfwTerminate();
}
