#pragma once

#include "Math.h"
#define ENEMY_MI 1.0
#define ENEMY_WALK_SPEED 2.0
#define ENEMY_JUMP_SPEED 5.0

#define DISTANCE_TO_FOLLOW 8.0

struct AI {
	AI();
	~AI();
	virtual void updateLogic(double dt, double &mass, Vector2d &pos, Vector2d &vel, Vector2d &force, double &health);	
};

struct PlayerAI: AI {
	void updateLogic(double dt, double &mass, Vector2d &pos, Vector2d &vel, Vector2d &force, double &health);	
};

struct EnemyStupidAI: AI {
	EnemyStupidAI();
	enum { IDLE = 0, JUMP = 1, LEFT = 2, RIGHT = 3} state;
	double waitingTime;
	double timeToChangeState;
	void updateLogic(double dt, double &mass, Vector2d &pos, Vector2d &vel, Vector2d &force, double &health);	
};