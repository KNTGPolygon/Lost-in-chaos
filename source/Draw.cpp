#include <iostream>
#include <fstream>

#include "Draw.h"

using namespace std;

Draw draw;

int DrawCreateBuffers(int w, int h)
{
	glGenTextures(DRAW_TARGETS,draw.texture);
	glGenFramebuffersEXT(DRAW_TARGETS, draw.target);

	for(int i=0;i<DRAW_TARGETS;i++)
	{
		glBindTexture( GL_TEXTURE_2D, draw.texture[i]);

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

		glTexImage2D( GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_FLOAT, 0 );

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, draw.target[i]);

		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, draw.texture[i], 0);
	}

	return 0;
}

void DrawDestroyBuffers()
{
	glDeleteFramebuffers(DRAW_TARGETS,draw.target);
	glDeleteTextures(DRAW_TARGETS,draw.texture);
}

bool loadFile(const char *fname, char **pout, int *plen)
{
    fstream f(fname,ios_base::in|ios_base::binary);
    if(!f.is_open())
        return false;
    f.seekg(0,ios::end);
    int s = f.tellg();
    f.seekg(0,ios::beg);
    (*pout) = new char[s+1];
    f.read(*pout,s);
    (*pout)[s] = 0;
    (*plen) = s+1;
    return true;
}

int GLLoadProgram(const char *fname_vert, const char *fname_frag, GLuint *pout, GLuint *vout, GLuint *fout)
{
    fstream f;
    GLchar *srcv, *srcf;
    int vl,fl;
    GLuint &prog = *pout, &vert = *vout, &frag = *fout;
    
    if(glShaderSource==0)
        return 1;

    prog = glCreateProgram();
    vert = glCreateShader(GL_VERTEX_SHADER);
    frag = glCreateShader(GL_FRAGMENT_SHADER);

    if(!loadFile(fname_vert,(char**)&srcv,&vl))
        return 2;
    if(!loadFile(fname_frag,(char**)&srcf,&fl))
    {
        delete [] srcv;
        return 3;
    }

    for(int i=0, j=0; ;i++)
    {
        srcv[j] = srcv[i];
        if(srcv[i]==0)
            break;
        if(srcv[i]=='\r')
            continue;
        j++, vl = j;
    }

    glShaderSource(vert,1,(const GLchar**)&srcv,(int*)&vl);
    glShaderSource(frag,1,(const GLchar**)&srcf,(int*)&fl);

    delete [] srcv;
    delete [] srcf;

    glCompileShader(vert);
    glCompileShader(frag);

    {
        GLint compiled;
        glGetShaderiv(vert, GL_COMPILE_STATUS, &compiled);
        if(compiled==GL_FALSE)
            return 4;
        glGetShaderiv(frag, GL_COMPILE_STATUS, &compiled);
        if(compiled==GL_FALSE)
            return 5;
    }

    glAttachShader(prog, vert);
    glAttachShader(prog, frag);
    glLinkProgram(prog);

    {
        GLint linked;
        glGetProgramiv(prog, GL_LINK_STATUS, &linked);
        if(!linked)
            return 6;
    }

    return 0;
}

void PrintShaderLog(GLuint shader)
{
	char *data = 0;
	GLint maxLength = 0, unused;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
	if(maxLength<=0)
		return;
	data = new char[maxLength];
	glGetShaderInfoLog(shader, maxLength, &unused, data);
	cout << "Shader " << shader << " log: " << data;
	delete [] data, data = 0;
}

int DrawInit()
{
	int err = GLLoadProgram("glsl/texture_light.vert.txt","glsl/texture_light.frag.txt",&draw.pTextureLight,&draw.vTextureLight,&draw.fTextureLight);
	if(err!=0)
	{
		cout << "program error " << err << "\n";
		switch(err)
		{
			case 4:
			PrintShaderLog(draw.vTextureLight);
			break;
			case 5:
			PrintShaderLog(draw.fTextureLight);
			break;
		}
		return -1;
	}
	glUseProgram(draw.pTextureLight);
	glUniform1i(glGetUniformLocation(draw.pTextureLight,"texture0"),0);
	glUniform1i(glGetUniformLocation(draw.pTextureLight,"texture1"),1);
	glUseProgram(0);
	return 0;
}

void DrawRelease()
{
	glDeleteProgram(draw.pTextureLight);
	glDeleteShader(draw.vTextureLight);
	glDeleteShader(draw.fTextureLight);
}
