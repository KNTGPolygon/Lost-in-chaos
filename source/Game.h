#pragma once

#include <vector>
#include <map>

#include "Math.h"

struct Entity
{
	struct
	{
		int type, flags;
		double vote;
	} entity;

	inline bool isWinner(double dt)
	{
		return (dt==entity.vote);
	}

	virtual void updateLogic(double dt);
	virtual double updateAuction(double dt);
	virtual void updatePhysics(double dt);

	virtual void draw();

	Entity();
	virtual ~Entity();
};

struct PropWall;

struct Game
{
	struct
	{
		double delta, speed;
	} time;

	struct
	{
		double gravity;
	} physics;

	std::vector<Entity*> vUpdate;
	std::vector<PropWall*> vWalls;

	void loadMap(const char *mapname);

	void update();
	void draw();

	Game();
	~Game();
};

extern Game game;

struct Collision
{
	Vector2d n;
	double p, v;
};
