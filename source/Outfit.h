#pragma once

#include <vector>
#include "Math.h"

#define OUTFIT_CONNECTIONS 4

struct Outfit
{
	double m, k0, k, f;
	Vector2d force, dv;

	struct Point
	{
		Vector2d p0, p, v, tc;
		bool dynamic;
		int connection[OUTFIT_CONNECTIONS];
		double conDis[OUTFIT_CONNECTIONS];

		Point();
	};

	std::vector<Point> vP;

	void flip();

	void update(double dt, Vector2d pos);

	void draw();

	Outfit();
};

extern Outfit outfit;
