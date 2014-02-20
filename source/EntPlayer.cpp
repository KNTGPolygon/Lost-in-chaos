#include "Window.h"
#include "Props.h"
#include "Resources.h"
#include "EntPlayer.h"
#include "EntBullet.h"
#include "Physics.h"

#define PLAYER_MASS 75.0
#define PLAYER_MOVE 1000.0
#define PLAYER_JUMP 1000.0

using namespace std;

void EntPlayer::updateLogic(double dt)
{
	game.playerPos = pos;
	{
		double mx, my;
		glfwGetCursorPos(window.handle,&mx,&my);
		aim.x = mx-(double)window.width*0.5;
		aim.y = (double)window.height*0.5-my;
		aim.normalize();
	}
	Vector2d move, gn;
	bool gnd = game.findGround(pos,vel,gn);

	if(window.key['D']>0)
		move-=gn.rotatedLeft(), dir = -1.0;
	if(window.key['A']>0)
		move+=gn.rotatedLeft(), dir = 1.0;

	if(window.key['W']==2 && gnd)
		vel+=Vector2d(0,PLAYER_JUMP_SPEED);

	if(move.magnitude2()>0.5)
		move.normalize();

	force = Vector2d(0,-PLAYER_MASS*game.physics.gravity);
	force += Friction(vel-move*PLAYER_WALK_SPEED,force,gn)*PLAYER_MI;

	vel+=(force/mass)*dt;

	if(window.key[500]==2)
	{
		EntBullet *bullet = new EntBullet;
		bullet->pos = pos+Vector2d(0,1.75);
		bullet->vel = vel+aim*32.0;
		game.vUpdate.push_back(bullet);
	}
}

double EntPlayer::updateAuction(double dt)
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

void EntPlayer::updatePhysics(double dt)
{
	pos+=vel*dt;
	game.playerPos = pos;
	if(isWinner(dt))
	for(int i=0;i<collisions.size();i++)
	{
		Collision &c = collisions[i];
		pos-=c.p*c.n;
		if(c.v<0.0)
		{
			double dv = (1.0*c.n*vel);
			vel-=dv*c.n;
			//if(abs(vel*c.n)<0.125)
			//	vel-=c.n*(vel*c.n);
		}
	}
	collisions.clear();
	game.camera = (15.0*game.camera + pos)/16.0;
}

void EntPlayer::draw()
{
	glColor4f(1,1,1,1);
	resources.drawSprite2 (1,pos,height*Vector2d(-Sign(aim.x),1));
}

EntPlayer::EntPlayer()
{
	pos = Vector2d(0,2);
	vel = Vector2d(0,0);
	mass = PLAYER_MASS;
	dir = 1.0;
	height = 1.81;
}

EntPlayer::~EntPlayer()
{
}
