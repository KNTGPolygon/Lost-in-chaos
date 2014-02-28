#pragma once

#include "Game.h"

#define ENTITY_ENEMY 3
#define ENEMY_MI 1.0

struct EntEnemy : Human
{
	void updateLogic(double dt);

	void draw(int mode);

	EntEnemy();
	~EntEnemy();
};
