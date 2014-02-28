#pragma once

#include "Game.h"

#define PLAYER_WALK_SPEED 4.0
#define PLAYER_JUMP_SPEED 5.0
#define PLAYER_MI 2.0

struct EntPlayer : Human
{
	Vector2d force;

	void updateLogic(double dt);
	void updatePhysics(double dt);

	void draw(int mode);

	EntPlayer();
	~EntPlayer();
};
