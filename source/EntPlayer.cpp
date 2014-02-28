#include "Window.h"
#include "Props.h"
#include "Resources.h"
#include "EntPlayer.h"
#include "EntBullet.h"
#include "Physics.h"
#include "Outfit.h"
#include "Draw.h"

#define PLAYER_MASS 75.0
#define PLAYER_MOVE 1000.0
#define PLAYER_JUMP 1000.0

using namespace std;

inline double Sgn(double x)
{
	return (x<0.0 ? -1.0 : 1.0);
}

void EntPlayer::updateLogic(double dt)
{
	game.playerPos = pos;
	{
		double mx, my;
		Vector2d aim2;
		glfwGetCursorPos(window.handle,&mx,&my);
		aim2.x = mx-(double)window.width*0.5;
		aim2.y = (double)window.height*0.5-my;
		aim2.normalize();
		//if(Sgn(aim2.x)!=Sgn(aim.x))
		//	outfit.flip();
		outfit.aim = aim = aim2;
	}
	Vector2d move, gn, v0 = vel;
	bool gnd = game.findGround(pos,vel,gn);

	if(window.key['D']>0)
		move-=gn.rotatedLeft();
	if(window.key['A']>0)
		move+=gn.rotatedLeft();

	if(window.key['W']==2 && gnd)
		vel+=Vector2d(0,PLAYER_JUMP_SPEED);

	if(move.magnitude2()>0.5)
		move.normalize();

	force = Vector2d(0,-PLAYER_MASS*game.physics.gravity);
	force += Friction(vel-move*PLAYER_WALK_SPEED,force,gn)*PLAYER_MI;

	//outfit.force = Vector2d(0,2.0*game.physics.gravity)*outfit.m;
	outfit.update(dt,pos);

	vel+=(force/mass)*dt;

	outfit.dv = v0-vel;

	if(window.key[500]==2)
	{
		EntBullet *bullet = new EntBullet;
		bullet->pos = pos+Vector2d(0,1.75);
		bullet->vel = vel+aim*32.0;
		game.insert(bullet);
	}
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
			double dv = (1.0*c.n*vel);
			vel-=dv*c.n;
			outfit.dv+=dv*c.n;
		}
	}
	collisions.clear();
}

void EntPlayer::draw(int mode)
{
	switch(mode)
	{
		case ENTITY_DRAW_COLOR:
		game.camera = (15.0*game.camera + pos)/16.0;
		glPushMatrix();
		glTranslated(pos.x,pos.y,0);
		outfit.draw();
		glPopMatrix();
		break;
		case ENTITY_DRAW_LIGHT:
		DrawBlendSubtract();
		DrawLightCircle(pos.x,pos.y+0.905,4,0.1,0.1,0.1);
		DrawBlendAdd();
		break;
	}
}

EntPlayer::EntPlayer()
{
	pos = Vector2d(0,2);
	vel = Vector2d(0,0);
	mass = PLAYER_MASS;
	height = 1.81;
}

EntPlayer::~EntPlayer()
{
}
