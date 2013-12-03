#include <iostream>

#include "Game.h"
#include "Props.h"
#include "EntPlayer.h"

using namespace std;

Game game;

void Entity::updateLogic(double dt)
{
}

double Entity::updateAuction(double dt)
{
	return dt;
}

void Entity::updatePhysics(double dt)
{
}

void Entity::draw()
{
}

Entity::Entity()
{
	entity.type = 0;
	entity.flags = 0;
}

Entity::~Entity()
{
}

void Game::loadMap(const char *mapname)
{
	vUpdate.push_back(new EntPlayer);
	vProps.push_back(new PropPlane(0,-7,0,1));
	vProps.push_back(new PropPlane(0,7,0,-1));
	vProps.push_back(new PropPlane(-9,0,1,0));
	vProps.push_back(new PropPlane(9,0,-1,0));
	vProps.push_back(new PropPlane(-5,-7,1,1));
	vProps.push_back(new PropPlane(5,-7,-1,1));
	vProps.push_back(new PropPlane(-5,7,1,-1));
	vProps.push_back(new PropPlane(5,7,-1,-1));
}

void Game::update()
{
	double dt = time.delta*time.speed;

	for(int i=0;i<vUpdate.size();i++)
	{
		vUpdate[i]->updateLogic(dt);
	}

	int count = 0;

	while(!IsZero(dt) && dt>0.0)
	{
		double vote = dt;
		for(int i=0;i<vUpdate.size();i++)
			vote = vUpdate[i]->entity.vote = min(max(vUpdate[i]->updateAuction(dt),0.0),vote);
		for(int i=0;i<vUpdate.size();i++)
			vUpdate[i]->updatePhysics(vote);
		dt-=vote;
		count++;
		//cout << "dt = "<< dt << " vote = "<<vote<<"\n";
	}
	if(count>8)
	{
		cout << "count="<<count<<"\n";
	}
}

void Game::draw()
{
	for(int i=0;i<vProps.size();i++)
		vProps[i]->draw();

	for(int i=0;i<vUpdate.size();i++)
	{
		vUpdate[i]->draw();
	}
}

Game::Game()
{
	time.delta = 1.0/60.0;
	time.speed = 1.0;

	physics.gravity = 9.81;
}

Game::~Game()
{
}
