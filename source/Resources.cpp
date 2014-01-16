#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include "Math.h"
#include "Resources.h"

Resources resources;

void Resources::init ()
{
	ilInit ();
	iluInit ();
	ilutRenderer (ILUT_OPENGL);

}

GLuint Resources::loadTexture (const char* nameOfFile)
{
	ILuint imageName;
	ilGenImages(1, &imageName);
	ilBindImage(imageName);
	
	if (!ilLoadImage(nameOfFile))
	{
		printf ("Houston, problem\n");
	}
	
	return ilutGLBindTexImage();
}

void Resources::loadBunchOfTextures ()
{
	resources.texture[0] = resources.loadTexture("texture/Sciana_drewno1.png");
	resources.texture[1] = resources.loadTexture("texture/Sciana_drewno2.png");
	resources.texture[2] = resources.loadTexture("texture/Sciana_drewno3.png");
	resources.sprite[0] = resources.loadTexture("texture/trollface.png");
}

void Resources::drawSprite (Vector2d pos)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture (GL_TEXTURE_2D, resources.sprite[TEX_WOODEN_PLANKS_1]);
	
	glEnable (GL_TEXTURE_2D);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	float n = 1;
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(pos.x-n,pos.y);
	glTexCoord2f(1, 0); glVertex2f(pos.x+n,pos.y);
	glTexCoord2f(1, 1); glVertex2f(pos.x+n,pos.y+2*n);
	glTexCoord2f(0, 1); glVertex2f(pos.x-n,pos.y+2*n);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void Resources::drawBackgroundTexture (double aspect)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture (GL_TEXTURE_2D, resources.texture[0]);
	
	glEnable (GL_TEXTURE_2D);
	glLoadIdentity ();
	
	float n = 4.0f;	
	
	glBegin(GL_QUADS);
	glTexCoord2f(-aspect*n, -n); glVertex2f(-1, -1.0);
	glTexCoord2f(aspect*n, -n); glVertex2f(1.0, -1.0);
	glTexCoord2f(aspect*n, n); glVertex2f(1.0, 1.0);
	glTexCoord2f(-aspect*n, n); glVertex2f(-1, 1.0);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
}
