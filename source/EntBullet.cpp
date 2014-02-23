#include "Window.h"
#include "EntBullet.h"
#include "Props.h"
#include "Resources.h"
#include "EntEnemy.h"
#include "Draw.h"

using namespace std;

void EntBullet::updateLogic(double dt)
{
	force = Vector2d(0,-mass*game.physics.gravity);
	vel+=(force/mass)*dt;
	life+=dt;
	if(life>10.0)
		entity.flags&=~ENTITY_ALIVE;
	track.push_back(Vector3d(pos.x,pos.y,time));
}

double EntBullet::updateAuction(double dt)
{
	if(life>10.0)
		return dt;
	Collision c;
	collisions.clear();
	for(int i=0;i<game.vProps.size();i++)
	{
		double t = dt;
		if(game.vProps[i]->calcImpact(pos,vel,t,c) && t<=dt)
		{
			if(t!=dt)
				collisions.clear();
			collisions.push_back(c);
			dt = t;
		}
	}
	for(int i=0;i<game.vUpdate.size();i++)
	if(game.vUpdate[i]->entity.type==ENTITY_ENEMY)
	{
		EntEnemy *e = (EntEnemy*)game.vUpdate[i];
		double t = (pos.x-(e->pos.x))/(e->vel.x-vel.x), t1 = (pos.y-e->pos.y-e->height)/(e->vel.y-vel.y), t2 = (pos.y-e->pos.y)/(e->vel.y-vel.y);
		if(t>=0.0 && min(t1,t2)<=t && max(t1,t2)>=t && t<=dt)
		{
			collisions.clear();
			ecol = e;
			dt = t;
		}
	}
	return dt;
}

void EntBullet::updatePhysics(double dt)
{
	pos+=vel*dt;
	if(isWinner(dt))
	{
		if(ecol!=0)
		{
			if(vel.magnitude2()>1.0)
				ecol->health-=vel.magnitude2();
			life = 1000.0;
			ecol = 0;
		}
		else
		for(int i=0;i<collisions.size();i++)
		{
			Collision &c = collisions[i];
			pos-=c.p*c.n;
			if(c.v<0.0)
			{
				double dv = (1.5*c.n*vel);
				vel-=dv*c.n;
			}
		}
	}
	collisions.clear();
	time+=dt;
	track.push_back(Vector3d(pos.x,pos.y,time));
}

void EntBullet::draw(int mode)
{
	if(mode!=ENTITY_DRAW_LIGHT)
		return;
	glBindTexture(GL_TEXTURE_2D,0);
	Vector2d n = -vel.normalized()*4.0;

	double a = min(time/0.1,1.0);

	glBegin(GL_LINE_STRIP);
	for(int i=0;i<track.size();i++)
	{
		glColor4f(1,0,0,a*exp(4.0*(track[i].z-time)));
		glVertex2f(track[i].x,track[i].y);
	}
	glEnd();

	DrawLightCircle(pos.x,pos.y,0.75*a,0.5,0,0);
}

EntBullet::EntBullet()
{
	pos = Vector2d(0,2);
	vel = Vector2d(0,0);
	mass = 1.0;
	life = 0.0;
	ecol = 0;
	time = 0.0;
}

EntBullet::~EntBullet()
{
}
