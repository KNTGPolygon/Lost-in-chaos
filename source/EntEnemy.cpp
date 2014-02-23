#include "Window.h"
#include "EntEnemy.h"
#include "Props.h"
#include "Resources.h"


using namespace std;

void EntEnemy::updateLogic(double dt)
{
	bool gnd = false;
	Vector2d gn;

	force = Vector2d(0,-mass*game.physics.gravity);
	for(int i=0;i<game.vProps.size();i++)
	{
		Vector2d n;
		bool var0 = game.vProps[i]->onEdge(pos,n), var1, var2, var3;
		Vector2d t = vel-(n*(vel*n));
		var1 = !IsZero(force*n);
		var2 = abs(vel*n)<0.01;
		var3 = !IsZero(t.magnitude2());
		if(var0)
		{
			gn = n;
			gnd = true;
		}
		if(var0 && var1 && var2 && var3 )
		{
			t = (force*n)*(t.normalized())*ENEMY_MI;
			force+=t;
		}
	}
	vel+=(force/mass)*dt;
	if(health<=0.0)
		entity.flags&=~ENTITY_ALIVE;
}

double EntEnemy::updateAuction(double dt)
{
	Collision c;
	collisions.clear();
	for(int i=0;i<game.vProps.size();i++)
	{
		double t = dt;
		if(game.vProps[i]->calcImpactLine(pos,height,vel,t,c) && t<=dt)
		{
			if(t!=dt)
				collisions.clear();
			collisions.push_back(c);
			dt = t;
		}
	}
	return dt;
}

void EntEnemy::updatePhysics(double dt)
{
	pos+=vel*dt;
	if(isWinner(dt))
	for(int i=0;i<collisions.size();i++)
	{
		Collision &c = collisions[i];
		pos-=c.p*c.n;
		if(c.v<0.0)
		{
			double dv = (1.25*c.n*vel);
			vel-=dv*c.n;
			if(abs(vel*c.n)<0.125)
				vel-=c.n*(vel*c.n);
		}
	}
	collisions.clear();
}

void EntEnemy::draw(int mode)
{
	if(mode!=ENTITY_DRAW_COLOR)
		return;
	glColor4f(0,1,0,1);
	resources.drawSprite2 (1,pos,height*Vector2d(-Sign(aim.x),1));
}

EntEnemy::EntEnemy()
{
	entity.type = ENTITY_ENEMY;
	pos = Vector2d(0,2);
	vel = Vector2d(0,0);
	aim = Vector2d(1,0);
	mass = 1.0;
	height = 1.81;
	health = 100.0;
}

EntEnemy::~EntEnemy()
{
}
