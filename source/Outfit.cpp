#include "Draw.h"
#include "Outfit.h"
#include "Game.h"
#include "Props.h"
#include "Resources.h"

using namespace std;

Outfit outfit;

Outfit::Point::Point()
{
	for(int i=0;i<OUTFIT_CONNECTIONS;i++)
		connection[i] = -1;
	dynamic = true;
}

void Outfit::update(double dt, Vector2d pos)
{
	Collision c, C;
	Vector2d F, a;
	double t, T, flip = Sign(aim.x);

	for(int i=0;i<vP.size();i++)
	{
		Point &p = vP[i];
		if(!p.dynamic)
		{
			p.p.x = p.p0.x*flip;
			p.p.y = p.p0.y;
		}
	}

	for(int i=0;i<vP.size();i++)
	{
		Point &p = vP[i];
		if(!p.dynamic)
			continue;
		F = force;
		for(int j=0;j<OUTFIT_CONNECTIONS;j++)
		if(p.connection[j]!=-1)
		{
			Point &r = vP[p.connection[j]];
			a = (r.p-p.p);
			F-=k*a.normalized()*(p.conDis[j]-a.magnitude());
		}
		a = (Vector2d(p.p0.x*flip,p.p0.y)-p.p);
		F+=k0*a*a.magnitude();
		F-=p.v*f;
		a = F/m;
		T = dt;
		p.v+=a*dt+dv;
		for(int j=0;j<game.vProps.size();j++)
		if(game.vProps[j]->calcImpact(pos+p.p,p.v,t,c) && t<T)
		{
			C = c;
			T = t;
		}
		p.p+=p.v*T;
		if(T<dt)
		{
			p.v-=(p.v*C.n)*C.n;
			p.p-=C.p*C.n;
			p.p+=p.v*(dt-T);
		}
	}

	force = Vector2d(0.0,0.0);
	dv = Vector2d(0.0,0.0);
}

#define T(i) glTexCoord2f(vP[i].tc.x,vP[i].tc.y)
#define V(i) T(i), glVertex2f(vP[i].p.x,vP[i].p.y)

void Outfit::draw()
{
	glBindTexture(GL_TEXTURE_2D,resources.texture[1]);
	glColor4f(1,1,1,1);
	glBegin(GL_TRIANGLES);
	V(0), V(1), V(3);
	V(1), V(2), V(4);
	V(1), V(3), V(4);
	V(6), V(5), V(4);
	V(6), V(3), V(4);
	glEnd();
}

Outfit::Outfit()
{
	m = 0.1;
	k0 = 4.5;
	k = 0.0;
	f = 0.5;
	
	Point p;
	
	p.p0 = p.p = Vector2d(-0.9,0);
	p.connection[0] = 1;
	p.connection[1] = 3;
	vP.push_back(p);

	p.p0 = p.p = Vector2d(0,0);
	p.connection[0] = 0;
	p.connection[1] = 2;
	p.connection[2] = 3;
	p.connection[3] = 4;
	vP.push_back(p);
	
	p.p0 = p.p = Vector2d(0.5,0);
	p.connection[0] = 1;
	p.connection[1] = 4;
	vP.push_back(p);

	p.p0 = p.p = Vector2d(-0.25,1.25);
	p.connection[0] = 0;
	p.connection[1] = 1;
	p.connection[1] = 2;
	p.dynamic = false;
	vP.push_back(p);

	p.p0 = p.p = Vector2d(0.5,1.25);
	p.dynamic = false;
	vP.push_back(p);

	p.p0 = p.p = Vector2d(0.45,2.0);
	p.dynamic = false;
	vP.push_back(p);

	p.p0 = p.p = Vector2d(0.0,2.0);
	p.dynamic = false;
	vP.push_back(p);

	for(int i=0;i<vP.size();i++)
	{
		Point &p = vP[i];
		p.tc.x = (1.0+p.p0.x)*0.5;
		p.tc.y = p.p0.y*0.5;
		for(int j=0;j<OUTFIT_CONNECTIONS;j++)
		if(p.connection[j]!=-1)
		{
			Point &r = vP[p.connection[j]];
			p.conDis[j] = (r.p-p.p).magnitude();
		}
	}
}
