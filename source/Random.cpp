#include "Math.h"
#include "Random.h"

using namespace std;

double Noise::get(int _x, int _y)
{
	unsigned int k;
	Random<int> rand;
	double r = 0.0, m = 1.0;
	long long i, j, x = _x, y = _y, d = 1;

	for(k=0;k<ls.size();k++)
	{
		Layer &l = ls[k];
		for(i=x-1;i<=x+1;i++)
		for(j=y-1;j<=y+1;j++)
		{
			rand.seed = int((i*i*l.a) + (i*l.b) + (j*j*l.c) + (j*l.d) + l.e);
			r += m*max(1.0-Vector2d(double(d*i-_x),double(d*j-_y)).magnitude()/m,0.0)*rand.getd();
		}
		x/=2;
		y/=2;
		m*=2.0, d*=2;
	}
	return r/w;
}

double Noise::getM(int _x, int _y, int mx, int my)
{
	unsigned int k;
	Random<int> rand;
	double r = 0.0, m = 1.0;
	_x+=mx, _y+=my;
	int i, j, x = _x, y = _y, d = 1, t, u;

	for(k=0;k<ls.size();k++)
	{
		Layer &l = ls[k];
		for(i=x-1;i<=x+1;i++)
		for(j=y-1;j<=y+1;j++)
		{
            t = i%mx;
            u = j%my;
			rand.seed = int((t*t*l.a) + (t*l.b) + (u*u*l.c) + (u*l.d) + l.e);
			r += m*max(1.0-Vector2d(double(d*i-_x),double(d*j-_y)).magnitude()/m,0.0)*rand.getd();
		}
		x/=2, mx/=2;
		y/=2, my/=2;
		m*=2.0, d*=2;
	}
	
	return r/w;
}

double Noise::getAA(int _x, int _y)
{
	double r = 0.0;
	int i, j;
	for(i=_x-1;i<=_x+1;i++)
	for(j=_y-1;j<=_y+1;j++)
		r+=get(i,j);
	return r/9.0;
}

void Noise::setDefault(int n, Random<int> &rand)
{
	int i;
	double d = 1.0;
	ls.clear();
	w = 0.0;
	for(i=0;i<n;i++)
	{
		Layer l;
		l.a = 0x4c1906;
		l.b = 0x5ac0db;
		l.c = 0x4307a7;
		l.d = 0x5f24f;
		l.e = rand.get();
		w+=d;
		d*=2.0;
		ls.push_back(l);
	}
}

double Noise3D::get(int __x, int __y, int __z)
{
	unsigned int l;
	double r = 0.0, m = 1.0;
	long long _x = (long long)2000000000+(long long)__x, _y = (long long)2000000000+(long long)__y, _z = (long long)2000000000+(long long)__z;
	long long i, j, k, x = _x, y = _y, z = _z, n = 1;

	for(l=0;l<ls.size();l++)
	{
		for(i=x-1;i<=x+1;i++)
		for(j=y-1;j<=y+1;j++)
		for(k=z-1;k<=z+1;k++)
		{
			rand.seed = int((i*i*a) + (i*b) + (j*j*c) + (j*d) + (k*k*e) + (k*f) + ls[l]);
			r += rand.getd()*max(m-Vector3d(double(n*i-_x),double(n*j-_y),double(n*k-_z)).magnitude(),0.0);
		}
		x/=2;
		y/=2;
		z/=2;
		m*=2.0, n*=2;
	}
	return r/w;
}

double Noise3D::getAA(int _x, int _y, int _z)
{
	double r = 0.0;
	int i, j, k;
	for(i=_x-1;i<=_x+1;i++)
	for(j=_y-1;j<=_y+1;j++)
	for(k=_z-1;k<=_z+1;k++)
		r+=get(i,j,k);
	return r/27.0;
}

void Noise3D::setDefault(int num, Random<int> &rand)
{
	int i;
	double n = 1.0;
	ls.clear();
	w = 0.0;
	a = 0x4c1906;
	b = 0x5ac0db;
	c = 0x4307a7;
	d = 0x5f24f;
	e = 0x829f;
	f = 0xf27ab1;
	for(i=0;i<num;i++)
	{
		w+=n;
		n*=2.0;
		ls.push_back(rand.gets());
	}
}
