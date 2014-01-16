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

int Resources::loadTexture (const char* nameOfFile, int index, int flags)
{
	ILuint imageName;
	ilGenImages(1, &imageName);
	ilBindImage(imageName);

	int w = ilGetInteger(IL_IMAGE_WIDTH);
	int h = ilGetInteger(IL_IMAGE_HEIGHT); 

	aspect[index] = (double)w/(double)h;

	if (!ilLoadImage(nameOfFile))
	{
		printf ("Houston, problem\n");
	}
	
	texture[index] = ilutGLBindTexImage();

	glBindTexture(GL_TEXTURE_2D,texture[index]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (flags&TEXTURE_REPEAT)!=0 ? GL_REPEAT : GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (flags&TEXTURE_REPEAT)!=0 ? GL_REPEAT : GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texture[index];
}

void Resources::loadBunchOfTextures ()
{
<<<<<<< HEAD
	resources.texture[0] = resources.loadTexture("texture/Sciana_drewno1.png");
	resources.texture[1] = resources.loadTexture("texture/Sciana_drewno2.png");
	resources.texture[2] = resources.loadTexture("texture/Sciana_drewno3.png");
	resources.sprite[0] = resources.loadTexture("texture/trollface.png");
=======
	resources.loadTexture("texture/background.jpg",0,TEXTURE_REPEAT);
	resources.loadTexture("texture/trollface.png",1,0);
	resources.loadTexture("texture/Sciana_drewno1.png",2,TEXTURE_REPEAT);
	resources.loadTexture("texture/Sciana_drewno2.png",3,TEXTURE_REPEAT);
	resources.loadTexture("texture/Sciana_drewno3.png",4,TEXTURE_REPEAT);
>>>>>>> b78c48bd94509823181e1aca834e94a24862a554
}

void Resources::drawSprite (int index, Vector2d pos)
{
<<<<<<< HEAD
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture (GL_TEXTURE_2D, resources.sprite[TEX_WOODEN_PLANKS_1]);
	
	glEnable (GL_TEXTURE_2D);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
=======
	glBindTexture (GL_TEXTURE_2D, resources.texture[index]);
>>>>>>> b78c48bd94509823181e1aca834e94a24862a554
	
	float n = 1;
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(pos.x-n,pos.y);
	glTexCoord2f(1, 0); glVertex2f(pos.x+n,pos.y);
	glTexCoord2f(1, 1); glVertex2f(pos.x+n,pos.y+2*n);
	glTexCoord2f(0, 1); glVertex2f(pos.x-n,pos.y+2*n);
	glEnd();
}

void Resources::drawBackgroundTexture (int index, float offset)
{
	glBindTexture (GL_TEXTURE_2D, texture[index]);

	float a = aspect[index];
		
	glBegin(GL_QUADS);
	glTexCoord2f(offset,0), glVertex2f(-a,-1);
	glTexCoord2f(offset,1), glVertex2f(-a,1);
	glTexCoord2f(offset+1,1), glVertex2f(a,1);
	glTexCoord2f(offset+1,0), glVertex2f(a,-1);
	glEnd();
}
