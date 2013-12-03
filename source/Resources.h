#pragma once

#include <GL/gl.h>

struct Resources
{
	GLuint texture;
	void init ();
	GLuint loadTexture (const char*);
	
	void setParameters ();
	void drawTexture (double aspect);
};

extern Resources resources;
