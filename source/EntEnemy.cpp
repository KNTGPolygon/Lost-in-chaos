#include "Window.h"
#include "EntEnemy.h"
#include "Props.h"
#include "Resources.h"
#include "Physics.h"

using namespace std;

void EntEnemy::updateLogic(double dt)
{
	Vector2d gn, force;
	bool gnd = game.findGround(pos,vel,gn);

	force = Vector2d(0,-mass*game.physics.gravity);
	force += Friction(vel,force,gn)*ENEMY_MI;

	vel+=(force/mass)*dt;

	if(health<=0.0)
		entity.flags&=~ENTITY_ALIVE;
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
	entity.flags|=ENTITY_UPDATE|ENTITY_DRAW;
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
