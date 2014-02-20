#pragma once

#include "Game.h"

struct EntEnemy;

struct EntBullet : Entity
{
	Vector2d pos, vel, force;
	double mass, life;


	EntEnemy *ecol;
	std::vector<Collision> collisions;

	void updateLogic(double dt);
	double updateAuction(double dt);
	void updatePhysics(double dt);

	void draw();

	EntBullet();
	~EntBullet();
};
