#pragma once

#include "Game.h"

struct EntEnemy;

struct EntBullet : Entity
{
	Vector2d pos, vel, force;
	double mass, life, time;

	EntEnemy *ecol;
	std::vector<Collision> collisions;
	std::vector<Vector3d> track;

	void updateLogic(double dt);
	double updateAuction(double dt);
	void updatePhysics(double dt);

	void draw(int mode);

	EntBullet();
	~EntBullet();
};
