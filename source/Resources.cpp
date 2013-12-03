#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

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
		printf("false\n");
	}
	
	return ilutGLBindTexImage();
}

void Resources::setParameters ()
{
	resources.texture = resources.loadTexture("../texture/Sciana_drewno2.png");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture (GL_TEXTURE_2D, resources.texture);
}

void Resources::drawTexture (double aspect)
{
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
