#pragma once

#include "Game.h"
#include "Math.h"

#define PROP_NONE 0
#define PROP_PLANE 1
#define PROP_CIRCLE 2
#define PROP_RECTANGLE 3

struct Prop
{
	int propType, unused0;
	Vector2d pos;

	virtual bool calcImpact(Vector2d p, Vector2d v, double &time, Collision &c);

	virtual void draw();

	Prop();
	virtual ~Prop();
};

struct PropPlane : Prop
{
	Vector2d normal;
	Vector3d color;

	bool calcImpact(Vector2d p, Vector2d v, double &time, Collision &c);

	void draw();

	PropPlane(double x, double y, double w, double h);
	PropPlane();
	~PropPlane();
};

struct PropCircle : Prop
{
	double radius;

	bool calcImpact(Vector2d p, Vector2d v, double &time, Collision &c);

	void draw();

	PropCircle(double x, double y, double r);
	PropCircle();
	~PropCircle();
};