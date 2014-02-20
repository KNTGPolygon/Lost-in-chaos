#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

e

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
	resources.loadTexture("texture/background.jpg",0,TEXTURE_REPEAT);
	resources.loadTexture("texture/trollface.png",1,0);
	resources.loadTexture("texture/Sciana_drewno1.png",2,TEXTURE_REPEAT);
	resources.loadTexture("texture/Sciana_drewno2.png",3,TEXTURE_REPEAT);
	resources.loadTexture("texture/Sciana_drewno3.png",4,TEXTURE_REPEAT);
}

void Resources::drawSprite (int index, Vector2d pos)
{
	glBindTexture (GL_TEXTURE_2D, resources.texture[index]);
	
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
