#pragma once

#include <GL/gl.h>

#define RESOURCE_TEXTURES 32

#define TEXTURE_REPEAT 1

struct Resources
{
	GLuint texture[RESOURCE_TEXTURES];
	double aspect[RESOURCE_TEXTURES];

	void init ();
	int loadTexture (const char *fname, int index, int flags);

	void loadBunchOfTextures ();
	
	void drawSprite (int index, Vector2d pos);
	void drawBackgroundTexture (int index, float offset);
};

extern Resources resources;
