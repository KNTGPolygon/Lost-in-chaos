#pragma once
#include <GL/gl.h>

<<<<<<< HEAD
#define SPRITE_TROLLFACE 0
#define TEX_WOODEN_PLANKS_1 0
#define TEX_WOODEN_PLANKS_2 1
#define TEX_WOODEN_PLANKS_3 2

struct Resources
{
	GLuint texture[99];
	GLuint sprite[99];
=======
#define RESOURCE_TEXTURES 32

#define TEXTURE_REPEAT 1

struct Resources
{
	GLuint texture[RESOURCE_TEXTURES];
	double aspect[RESOURCE_TEXTURES];

>>>>>>> b78c48bd94509823181e1aca834e94a24862a554
	void init ();
	int loadTexture (const char *fname, int index, int flags);

	void loadBunchOfTextures ();
	
	void drawSprite (int index, Vector2d pos);
	void drawBackgroundTexture (int index, float offset);
};

extern Resources resources;
