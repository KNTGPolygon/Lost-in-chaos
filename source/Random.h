#pragma once

#include <vector>
#include <cmath>

//#include "Thread.h"

template<typename T> struct Random
{
	T seed, a, b;

	T get()
	{
		seed = a*seed + b;
		return seed<0 ? -seed : seed;
	}

	T gets()
	{
		seed = a*seed + b;
		return seed;
	}

	float getf()
	{
		return float(get()%1025)/1024.0f;
	}

	double getd()
	{
		static const int pi = 1024*1024+1;
		static const double pd = 1024.0*1024.0;
		return double(get()%pi)/pd;
	}

	float getf2()
	{
		union
		{
			int i;
			float f;
		};
		i = (((int)0b01111111)<<23)|(gets()&0x7fffff);
		return f-1.0;
	}

	double getd2()
	{
		union
		{
			long long l;
			double d;
		};
		l = (((long long)0b01111111111)<<52)|(gets()&0x0fffffffffffff);
		return d-1.0;
	}

	Random()
	{
		a = 8253729;
		b = 2396403;
	}
};

/*template<typename T> struct RandomMT : Random<T>
{
    Mutex mt;
    
    T sget()
    {
        T r;
        mt.lock();
        r = Random<T>::get();
        mt.unlock();
        return r;
    }
    
    T sgets()
    {
        T r;
        mt.lock();
        r = Random<T>::gets();
        mt.unlock();
        return r;
    }
    
    RandomMT() : Random<T>()
    {
    }
};*/

struct Noise
{
	struct Layer
	{
		int a, b, c, d, e;
	};

	double w;
	std::vector<Layer> ls;

	double get(int x, int y);
	double getAA(int x, int y);
	double getM(int x, int y, int mx, int my);
	
	void setDefault(int n, Random<int> &rand);
};

struct Noise3D
{
	Random<int> rand;
	int a, b, c, d, e, f;

	double w;
	std::vector<int> ls;

	double get(int x, int y, int z);
	double getAA(int x, int y, int z);
	void setDefault(int n, Random<int> &rand);
};
