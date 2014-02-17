#pragma once

#include "Game.h"

#define PLAYER_MI 1.0

struct EntPlayer : Entity
{
	Vector2d pos, vel, force, aim;
	double mass, dir, height, health;

	std::vector<Collision> collisions;

	void updateLogic(double dt);
	double updateAuction(double dt);
	void updatePhysics(double dt);

	void draw();

	EntPlayer();
	~EntPlayer();
};
