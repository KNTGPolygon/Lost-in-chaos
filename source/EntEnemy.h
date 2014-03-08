#pragma once

#include "Game.h"

#define ENTITY_ENEMY 3

struct EntEnemy : Human
{
	Vector2d force;
	int direction;
	void updateLogic(double dt);

	void draw(int mode);

	EntEnemy();
	~EntEnemy();
};
