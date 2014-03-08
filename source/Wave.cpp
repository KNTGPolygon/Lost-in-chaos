#include <iostream>
#include <fstream>
#include <cmath>

#include "Wave.h"

using namespace std;

int WaveData::load(const char *fname)
{
	if(data!=0)
		return -1;
	WaveHeader head;
	fstream f(fname,ios_base::in|ios_base::binary);
	if(!f.is_open())
	{
		f.close();
		return -2;
	}
	f.seekg(0,f.end);
	long long fsize = f.tellg();
	f.seekg(0,f.beg);
	if(fsize<=44ll || fsize>1000000000)
	{
		f.close();
		return -3;
	}
	f.read((char*)&head,44);
	if(head.chunkID!=1179011410)
	{
		f.close();
		return -4;
	}
	if(head.chunkSize!=fsize-8)
	{
		f.close();
		return -5;
	}
	if(head.format!=1163280727)
	{
		f.close();
		return -6;
	}
	if(head.subchunk1ID!=544501094)
	{
		f.close();
		return -7;
	}
	if(head.subchunk1Size!=16)
	{
		f.close();
		return -8;
	}
	if(head.audioFormat!=1)
	{
		f.close();
		return -9;
	}
	if(head.numChannels!=1 && head.numChannels!=2)
	{
		f.close();
		return -10;
	}
	if(head.sampleRate<=0)
	{
		f.close();
		return -11;
	}
	if(head.bitsPerSample!=8 && head.bitsPerSample!=16)
	{
		f.close();
		return -12;
	}
	if(head.subchunk2ID!=1635017060)
	{
		f.seekg(head.subchunk2Size+f.tellg(),f.beg);
		f.read((char*)&head.subchunk2ID,8);
		if(head.subchunk2ID!=1635017060)
		{
			f.close();
			return -13;
		}
	}
	/*if(head.subchunk2Size!=fsize-44)
	{
		f.close();
		return -14;
	}*/
	try
	{
		channels = head.numChannels;
		switch(head.bitsPerSample)
		{
			case 8:
			format = (channels==1 ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8);
			size = head.subchunk2Size;
			data = new char[size];
			break;
			case 16:
			format = (channels==1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16);
			size = head.subchunk2Size;
			data = new short[size/2];
			break;
		}
		sampleRate = head.sampleRate;
	}
	catch(bad_alloc &ba)
	{
		f.close();
		return -15;
	}
	int count = 0;
	for(int i=0;i<8 && count<size;i++)
		f.read((char*)data+count,size-count), count+=f.gcount();
	if(count!=size)
	{
		f.close();
		return -16;
	}
	f.close();
	return 0;
}

int WaveData::dataToBuffer(ALuint buffer)
{
	if(data==0)
		return -1;
	alBufferData(buffer,format,data,size,sampleRate);
	return 0;
}

void WaveData::clear()
{
	if(data==0)
		return;
	switch(format)
	{
		case AL_FORMAT_MONO8:
		case AL_FORMAT_STEREO8:
		delete [] (char*)data;
		break;
		case AL_FORMAT_MONO16:
		case AL_FORMAT_STEREO16:
		delete [] (short*)data;
		break;
		default:
		delete [] (char*)data;
		break;
	}
	format = 0;
	channels = 0;
	sampleRate = 0;
	size = 0;
	data = 0;
}

WaveData::WaveData()
{
	format = 0;
	channels = 0;
	sampleRate = 0;
	size = 0;
	data = 0;
}

WaveData::~WaveData()
{
	clear();
}
