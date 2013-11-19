#pragma once

#include "Game.h"

struct EntPlayer : Entity
{
	Vector2d pos, vel, force;
	double mass;

	std::vector<Collision> collisions;

	void updateLogic(double dt);
	double updateAuction(double dt);
	void updatePhysics(double dt);

	void draw();

	EntPlayer();
	~EntPlayer();
};
