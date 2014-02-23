#pragma once

#include <GL/glew.h>
#include "Math.h"

#define DRAW_TARGETS 2
#define DRAW_COLOR 0
#define DRAW_LIGHT 1

struct Draw
{
	GLuint texture[DRAW_TARGETS];
	GLuint target[DRAW_TARGETS];
	GLuint pTextureLight, vTextureLight, fTextureLight;
};

extern Draw draw;

inline void DrawBlendNormal()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
}

inline void DrawBlendAdd()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBlendEquation(GL_FUNC_ADD);
}

inline void DrawBlendSubtract()
{
	//glBlendFunc(GL_ONE, GL_ONE);
	glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE, GL_ZERO, GL_ONE);
	glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
}

inline void DrawTargetWindow()
{
	glUseProgram(0);
	DrawBlendNormal();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

inline void DrawTargetColor()
{
	glUseProgram(0);
	DrawBlendNormal();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,draw.target[DRAW_COLOR]);
}

inline void DrawTargetLight()
{
	glUseProgram(0);
	DrawBlendAdd();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,draw.target[DRAW_LIGHT]);
}

int DrawCreateBuffers(int w, int h);
void DrawDestroyBuffers();

int DrawInit();
void DrawRelease();

inline void DrawLightCircle(double x, double y, double z, double r, double g, double b)
{
	glBegin(GL_TRIANGLES);
	for(int i=0;i<16;i++)
	{
		double a = 2.0*PI*(double)i/16.0;
		double c = 2.0*PI*(double)(i+1)/16.0;
		glColor4f(r,g,b,1);
		glVertex2f(x,y);
		glColor4f(0,0,0,0);
		glVertex2f(x+z*cos(a),y+z*sin(a));
		glVertex2f(x+z*cos(c),y+z*sin(c));
	}
	glEnd();
}
