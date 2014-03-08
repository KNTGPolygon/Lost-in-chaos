#pragma once

#include <vector>
#include <string>
#include <map>
#include <random>

#include "AI.h"
#include "Math.h"

#define GAME_VIEW_RADIUS 8.0

#define ENTITY_ALIVE 1
#define ENTITY_UPDATE 2
#define ENTITY_DRAW 8
#define ENTITY_PROP 32
#define ENTITY_HUMAN 64

#define ENTITY_DRAW_COLOR 0
#define ENTITY_DRAW_LIGHT 1

struct Entity
{
	struct
	{
		int type, flags, ie, ip, iu, id;
		double vote;
	} entity;

	Vector2d pos, vel;

	inline bool isWinner(double dt)
	{
		return (dt==entity.vote);
	}

	virtual void updateLogic(double dt);
	virtual double updateAuction(double dt);
	virtual void updatePhysics(double dt);

	virtual void draw(int mode);

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

	std::vector<Entity*> vEntity;
	std::vector<Prop*> vProps;
	std::vector<Entity*> vUpdate, vDraw;

	std::string nextlevel;

	std::random_device rd;

	inline int rand(int a, int b)
	{
		return a+(rd()-rd.min())%(b-a+1);
	}

	void insert(Entity *e);
	void erase(Entity *e);

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

struct Prop : Entity
{
	int propType, unused0;

	virtual bool calcImpact(Vector2d p, Vector2d v, double &time, Collision &c);
	virtual bool calcImpactLine(Vector2d p, double height, Vector2d v, double &time, Collision &c);
	virtual bool onEdge(Vector2d p, Vector2d &n);

	Prop();
	virtual ~Prop();
};

struct Human : Entity
{
	AI* ai;
	Vector2d aim;
	double mass, height, health, maxHealth;
	std::vector<Collision> collisions;

	double updateAuction(double dt);
	void updatePhysics(double dt);

	Human();
	virtual ~Human();
};
