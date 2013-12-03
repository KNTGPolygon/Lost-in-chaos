#pragma once

#include <GL/gl.h>

struct Resources
{
	GLuint texture[10];
	void init ();
	GLuint loadTexture (const char*);
	
	void drawSprite (Vector2d pos);
	void loadBunchOfTextures ();
	void drawBackgroundTexture (double aspect);
};

extern Resources resources;
