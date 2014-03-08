#pragma once

#include <AL/al.h>

struct WaveHeader
{
	int chunkID;
	int chunkSize;
	int format;
	int subchunk1ID;
	int subchunk1Size;
	short audioFormat;
	short numChannels;
	int sampleRate;
	int byteRate;
	short blockAlign;
	short bitsPerSample;
	int subchunk2ID;
	int subchunk2Size;
	int unused;
};

struct WaveData
{
	ALuint format, channels, sampleRate, size;

	void *data;

	int dataToBuffer(ALuint buffer);

	int load(const char *fname);

	void clear();

	WaveData();
	~WaveData();
};
