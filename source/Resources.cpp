#include <string>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include "Math.h"
#include "Resources.h"

#include <AL/alc.h>

using namespace std;

Resources resources;

ALCdevice *alDev;
ALCcontext *alCtx;

int Resources::init ()
{
	ilInit ();
	iluInit ();
	ilutRenderer (ILUT_OPENGL);

	alDev = alcOpenDevice(NULL);
	if(!alDev)
		return -4;
	alCtx = alcCreateContext(alDev, NULL);
	alcMakeContextCurrent(alCtx);
	if(!alCtx)
		return -5;
	alGenBuffers(AUDIO_BUFFERS, buffers);
	alGenSources(AUDIO_SOURCES, sources);
	return 0;
}

void Resources::release()
{
	alDeleteSources(AUDIO_SOURCES, sources);
	alDeleteBuffers(AUDIO_BUFFERS, buffers);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(alCtx);
	alcCloseDevice(alDev);
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

void Resources::load ()
{
	resources.loadTexture("texture/mapaja.jpg",31,TEXTURE_REPEAT);
	resources.loadTexture("texture/background.jpg",0,TEXTURE_REPEAT);
	resources.loadTexture("texture/postac.png",1,0);
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

void Resources::drawSprite2 (int index, Vector2d pos, Vector2d n)
{
	glBindTexture (GL_TEXTURE_2D, resources.texture[index]);

	n*=0.5;
	
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(pos.x-n.x,pos.y);
	glTexCoord2f(1, 0); glVertex2f(pos.x+n.x,pos.y);
	glTexCoord2f(1, 1); glVertex2f(pos.x+n.x,pos.y+2*n.y);
	glTexCoord2f(0, 1); glVertex2f(pos.x-n.x,pos.y+2*n.y);
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

void ogg_stream::open(string path)
{
    int result;
    
    if(!(oggFile = fopen(path.c_str(), "rb")))
        return;//throw string("Could not open Ogg file.");

    if((result = ov_open(oggFile, &oggStream, NULL, 0)) < 0)
    {
        fclose(oggFile);
        
        return;//throw string("Could not open Ogg stream. ") + errorString(result);
    }

    vorbisInfo = ov_info(&oggStream, -1);
    vorbisComment = ov_comment(&oggStream, -1);

    if(vorbisInfo->channels == 1)
        format = AL_FORMAT_MONO16;
    else
        format = AL_FORMAT_STEREO16;
        
        
    alGenBuffers(2, buffers);
    check();
    alGenSources(1, &source);
    check();
    
    alSource3f(source, AL_POSITION,        0.0, 0.0, 0.0);
    alSource3f(source, AL_VELOCITY,        0.0, 0.0, 0.0);
    alSource3f(source, AL_DIRECTION,       0.0, 0.0, 0.0);
    alSourcef (source, AL_ROLLOFF_FACTOR,  0.0          );
    alSourcei (source, AL_SOURCE_RELATIVE, AL_TRUE      );
}

void ogg_stream::release()
{
    alSourceStop(source);
    empty();
    alDeleteSources(1, &source);
    check();
    alDeleteBuffers(1, buffers);
    check();

    ov_clear(&oggStream);
}

void ogg_stream::display()
{
    cout
        << "version         " << vorbisInfo->version         << "\n"
        << "channels        " << vorbisInfo->channels        << "\n"
        << "rate (hz)       " << vorbisInfo->rate            << "\n"
        << "bitrate upper   " << vorbisInfo->bitrate_upper   << "\n"
        << "bitrate nominal " << vorbisInfo->bitrate_nominal << "\n"
        << "bitrate lower   " << vorbisInfo->bitrate_lower   << "\n"
        << "bitrate window  " << vorbisInfo->bitrate_window  << "\n"
        << "\n"
        << "vendor " << vorbisComment->vendor << "\n";
        
    for(int i = 0; i < vorbisComment->comments; i++)
        cout << "   " << vorbisComment->user_comments[i] << "\n";
        
    cout << endl;
}

bool ogg_stream::playback()
{
    if(playing())
        return true;
        
    if(!stream(buffers[0]))
        return false;
        
    if(!stream(buffers[1]))
        return false;
    
    alSourceQueueBuffers(source, 2, buffers);
    alSourcePlay(source);
    
    return true;
}

bool ogg_stream::playing()
{
    ALenum state;
    
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    
    return (state == AL_PLAYING);
}

bool ogg_stream::update()
{
    int processed;
    bool active = true;

    alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);

    while(processed--)
    {
        ALuint buffer;
        
        alSourceUnqueueBuffers(source, 1, &buffer);
        check();

        active = stream(buffer);

        alSourceQueueBuffers(source, 1, &buffer);
        check();
    }

    int state;
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    if(state!=AL_PLAYING)
    	alSourcePlay(source);

    return active;
}

bool ogg_stream::stream(ALuint buffer)
{
    char pcm[BUFFER_SIZE];
    int  size = 0;
    int  section;
    int  result;

    while(size < BUFFER_SIZE)
    {
        result = ov_read(&oggStream, pcm + size, BUFFER_SIZE - size, 0, 2, 1, &section);
    
        if(result > 0)
            size += result;
        else
            if(result < 0)
                throw errorString(result);
            else
                break;
    }
    
    if(size == 0)
        return false;
        
    alBufferData(buffer, format, pcm, size, vorbisInfo->rate);
    check();
    
    return true;
}

void ogg_stream::empty()
{
    int queued;
    
    alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);
    
    while(queued--)
    {
        ALuint buffer;
    
        alSourceUnqueueBuffers(source, 1, &buffer);
        check();
    }
}

void ogg_stream::check()
{
	int error = alGetError();

	if(error != AL_NO_ERROR)
		throw string("OpenAL error was raised.");
}

string ogg_stream::errorString(int code)
{
    switch(code)
    {
        case OV_EREAD:
            return string("Read from media.");
        case OV_ENOTVORBIS:
            return string("Not Vorbis data.");
        case OV_EVERSION:
            return string("Vorbis version mismatch.");
        case OV_EBADHEADER:
            return string("Invalid Vorbis header.");
        case OV_EFAULT:
            return string("Internal logic fault (bug or heap/stack corruption.");
        default:
            return string("Unknown Ogg error.");
    }
}
