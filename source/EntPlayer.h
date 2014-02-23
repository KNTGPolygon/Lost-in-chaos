#pragma once

#include "Game.h"

#define PLAYER_WALK_SPEED 2.0
#define PLAYER_JUMP_SPEED 5.0
#define PLAYER_MI 2.0

struct EntPlayer : Entity
{
	Vector2d pos, vel, force, aim, deltaV;
	double mass, dir, height, health;

	std::vector<Collision> collisions;

	void updateLogic(double dt);
	double updateAuction(double dt);
	void updatePhysics(double dt);

	void draw(int mode);

	EntPlayer();
	~EntPlayer();
};
