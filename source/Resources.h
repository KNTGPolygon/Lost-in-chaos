#pragma once

#include <string>
#include <GL/gl.h>
#include <AL/al.h>

#define RESOURCE_TEXTURES 32
#define AUDIO_BUFFERS 32
#define AUDIO_SOURCES 64

#define TEXTURE_REPEAT 1
#define TEX_CAT 6

struct Resources
{
	GLuint texture[RESOURCE_TEXTURES];
	double aspect[RESOURCE_TEXTURES];
	ALuint buffers[AUDIO_BUFFERS];
	ALuint sources[AUDIO_SOURCES];


	int init ();
	void release();

	int loadTexture (const char *fname, int index, int flags);

	void load ();
	
	void drawSprite (int index, Vector2d pos);
	void drawSprite2 (int index, Vector2d pos, Vector2d n);
    void drawSprite2c (int index, Vector2d pos, Vector2d n);
	void drawBackgroundTexture (int index, float offset);
};

extern Resources resources;

#include <ogg/ogg.h>
#include <vorbis/vorbisfile.h>



#define BUFFER_SIZE (4096 * 4)



class ogg_stream
{
    public:

        void open(std::string path);
        void release();
        void display();
        bool playback();
        bool playing();
        bool update();

    protected:

        bool stream(ALuint buffer);
        void empty();
        void check();
        std::string errorString(int code);

    private:

        FILE*           oggFile;
        OggVorbis_File  oggStream;
        vorbis_info*    vorbisInfo;
        vorbis_comment* vorbisComment;

        ALuint buffers[2];
        ALuint source;
        ALenum format;
};
