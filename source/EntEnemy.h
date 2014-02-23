#pragma once

#include "Game.h"

#define ENTITY_ENEMY 3
#define ENEMY_MI 1.0

struct EntEnemy : Entity
{
	Vector2d pos, vel, force, aim;
	double mass, health, height;

	std::vector<Collision> collisions;

	void updateLogic(double dt);
	double updateAuction(double dt);
	void updatePhysics(double dt);

	void draw(int mode);

	EntEnemy();
	~EntEnemy();
};
