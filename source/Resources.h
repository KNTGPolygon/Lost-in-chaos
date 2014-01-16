#pragma once
#include <GL/gl.h>

#define SPRITE_TROLLFACE 0
#define TEX_WOODEN_PLANKS_1 0
#define TEX_WOODEN_PLANKS_2 1
#define TEX_WOODEN_PLANKS_3 2

struct Resources
{
	GLuint texture[99];
	GLuint sprite[99];
	void init ();
	GLuint loadTexture (const char*);
	
	void drawSprite (Vector2d pos);
	void loadBunchOfTextures ();
	void drawBackgroundTexture (double aspect);
};

extern Resources resources;
