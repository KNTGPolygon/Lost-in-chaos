#pragma once

#include <vector>
#include <string>
#include <map>

#include "Math.h"

#define ENTITY_ALIVE 1

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

struct Prop;

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

	Vector2d camera, playerPos;

	std::vector<Entity*> vUpdate;
	std::vector<Prop*> vProps;

	std::string nextlevel;

	bool findGround(Vector2d pos, Vector2d vel, Vector2d &outn);

	void loadMap(const char *mapname);

	void update();

	void drawLight();
	void draw();

	void clear();

	Game();
	~Game();
};

extern Game game;

struct Collision
{
	Vector2d n;
	double p, v;
};
