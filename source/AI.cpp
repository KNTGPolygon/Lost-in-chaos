#include "AI.h"
#include "Game.h"
#include "Props.h"
#include "Resources.h"
#include "Physics.h"
#include "EntPlayer.h"
#include <cstdlib>
#include <ctime>

void AI::updateLogic(double dt, double &mass, Vector2d &pos, Vector2d &vel, Vector2d &force, double &health)
{
	// ???
}

AI::AI()
{
	srand(time(NULL));
}

AI::~AI()
{
}

void PlayerAI::updateLogic(double dt, double &mass, Vector2d &pos, Vector2d &vel, Vector2d &force, double &health)
{
}

EnemyStupidAI::EnemyStupidAI(): state(IDLE), waitingTime(0), timeToChangeState(2.0)
{

}

void EnemyStupidAI::updateLogic(double dt, double &mass, Vector2d &pos, Vector2d &vel, Vector2d &force, double &health)
{
	if(waitingTime > timeToChangeState)
	{
		waitingTime = 0;
		int r = rand() % 4;
		if(r == 0) state = IDLE;
		else if(r == 1) state = JUMP;
		else if(r == 2) state = LEFT;
		else if(r == 3) state = RIGHT;
		if(state == JUMP) timeToChangeState = 0.1;
		else timeToChangeState = double(rand() % 100) / 50;
	}
	waitingTime += dt;

	Vector2d gn, move;
	bool gnd = game.findGround(pos, vel, gn);

	if(state == RIGHT)
		move-=gn.rotatedLeft();
	if(state == LEFT)
		move+=gn.rotatedLeft();

	if(state == JUMP && gnd) {
		vel+=Vector2d(0,ENEMY_JUMP_SPEED);
		state = IDLE;
	}

	force = Vector2d(0, -mass * game.physics.gravity);
	force += Friction(vel-move*ENEMY_WALK_SPEED, force, gn) * ENEMY_MI;

	vel += (force / mass) * dt;

}