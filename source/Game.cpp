#include <iostream>
#include <fstream>
#include <GL/glew.h>

#include "Game.h"
#include "Props.h"
#include "EntPlayer.h"
#include "EntEnemy.h"
#include "Resources.h"
#include "Window.h"

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
	entity.flags = ENTITY_ALIVE;
}

Entity::~Entity()
{
}

void Game::loadMap(const char *mapname)
{
	fstream f(("map/"+string(mapname)+".txt").c_str(),ios_base::in);

	if(!f.is_open())
	{
		cout << "error: map `" << mapname << "` not found\n";
		return;
	}

	while(true)
	{
		string in;
		if(!(f>>in) || f.eof())
			break;
		if(in=="plane")
		{
			double x, y, nx, ny;
			f >> x >> y >> nx >> ny;
			vProps.push_back(new PropPlane(x,y,nx,ny));
		}
		else
		if(in=="circle")
		{
			double x, y, r;
			f >> x >> y >> r;
			vProps.push_back(new PropCircle(x,y,r));
		}
		else
		if(in=="player")
		{
			double x, y;
			f >> x >> y;
			EntPlayer *player = new EntPlayer;
			player->pos = Vector2d(x,y);
			vUpdate.push_back(player);
		}
		else
		if(in=="enemy")
		{
			double x, y, d;
			f >> x >> y >> d;
			EntEnemy *enemy = new EntEnemy;
			enemy->pos = Vector2d(x,y);
			enemy->aim = Vector2d(d,0).normalized();
			vUpdate.push_back(enemy);
		}
		else
		if(in=="nextlevel")
			f >> game.nextlevel;
		else
			cout << "wtf `" << in << "`\n";
	}

	f.close();
}

void Game::update()
{
	double dt = time.delta*time.speed;

	int i, j;

	for(i=0, j=0;i<vUpdate.size();i++)
	{
		vUpdate[i]->updateLogic(dt);
		if(vUpdate[i]->entity.flags&ENTITY_ALIVE)
			vUpdate[j] = vUpdate[i], j++;
		else
			delete vUpdate[i];
	}

	if(i!=j)
	{
		if(j<=0)
			vUpdate.clear();
		else
			vUpdate.resize(j);
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
	double aspect = (double)window.width/(double)window.height;

	glLoadIdentity();

	//glColor4f(1,1,1,1);
	//resources.drawBackgroundTexture (0,game.camera.x/128.0);

	glBindTexture(GL_TEXTURE_2D,0);
	glBegin(GL_QUADS);
	glColor4f(0,0,0.2,1);
	glVertex2f(-1,1);
	glVertex2f(1,1);
	glColor4f(0,0,0.05,1);
	glVertex2f(1,-1);
	glVertex2f(-1,-1);
	glEnd();

	glOrtho(-aspect*8.0,aspect*8.0,-8,8,-1,1);
	glTranslated(-game.camera.x,-game.camera.y,0);
	for(int i=0;i<vProps.size();i++)
		vProps[i]->draw();

	for(int i=0;i<vUpdate.size();i++)
	{
		vUpdate[i]->draw();
	}
}

void Game::clear()
{
	for(int i=0;i<vUpdate.size();i++)
		delete vUpdate[i];
	vUpdate.clear();
	for(int i=0;i<vProps.size();i++)
		delete vProps[i];
	vProps.clear();
}

Game::Game()
{
	time.delta = 1.0/100.0;
	time.speed = 1.0;

	physics.gravity = 9.81;
}

Game::~Game()
{
}
