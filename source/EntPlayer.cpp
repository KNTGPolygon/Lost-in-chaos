#include "Window.h"
#include "EntPlayer.h"
#include "Props.h"
#include "Resources.h"

#define PLAYER_MASS 75.0
#define PLAYER_MOVE 1000.0
#define PLAYER_JUMP 1000.0

using namespace std;

void EntPlayer::updateLogic(double dt)
{
	Vector2d move;
	if(window.key['D']>0)
		move.x+=1.0;
	if(window.key['A']>0)
		move.x-=1.0;
	if(window.key['W']>0)
		move.y+=1.0;
	if(window.key['S']>0)
		move.y-=1.0;
	if(move.magnitude2()>0.5)
		move.normalize();
	force = Vector2d(0,-PLAYER_MASS*game.physics.gravity)+PLAYER_MOVE*move;
	vel+=(force/mass)*dt;
	if(window.key['P']==2)
		cout << "p=" << pos << " v=" << vel << "\n", vel = Vector2d(0,0);
}

double EntPlayer::updateAuction(double dt)
{
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
	return dt;
}

void EntPlayer::updatePhysics(double dt)
{
	pos+=vel*dt;
	if(isWinner(dt))
	for(int i=0;i<collisions.size();i++)
	{
		Collision &c = collisions[i];
		pos-=c.p*c.n;
		if(c.v<0.0)
		{
			double dv = (1.5*c.n*vel);
			vel-=dv*c.n;
			if(abs(vel*c.n)<0.25)
				vel-=c.n*(vel*c.n);
		}
	}
	collisions.clear();
	game.camera = (31.0*game.camera + pos)/32.0;
}

void EntPlayer::draw()
{
	resources.drawSprite (pos);
}

EntPlayer::EntPlayer()
{
	pos = Vector2d(0,2);
	vel = Vector2d(0,0);
	mass = PLAYER_MASS;
}

EntPlayer::~EntPlayer()
{
}
