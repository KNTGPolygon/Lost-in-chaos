#pragma once

#include "Game.h"
#include "Math.h"

struct PropWall
{
	Vector2d pos, normal;
	Vector3d color;

	void draw();

	bool calcImpact(Vector2d p, Vector2d v, double &time, Collision &c);

	PropWall(double x, double y, double w, double h);
	PropWall();
	~PropWall();
};
