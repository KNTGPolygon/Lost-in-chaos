#include <iostream>
#include <fstream>
#include <GL/glew.h>

#include "Game.h"
#include "Props.h"
#include "EntPlayer.h"
#include "EntEnemy.h"
#include "Resources.h"
#include "Window.h"
#include "Draw.h"

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

void Entity::draw(int mode)
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

void Game::insert(Entity *e)
{
	e->entity.ie = vEntity.size();
	vEntity.push_back(e);
	if(e->entity.flags&ENTITY_PROP)
		e->entity.ip = vProps.size(), vProps.push_back((Prop*)e);
	if(e->entity.flags&ENTITY_UPDATE)
		e->entity.iu = vUpdate.size(), vUpdate.push_back(e);
	if(e->entity.flags&ENTITY_DRAW)
		e->entity.id = vDraw.size(), vDraw.push_back(e);
}

#define ERASE(v,i,e) v.back()->entity.i = e->entity.i, swap(v[e->entity.i],v.back()), v.pop_back()

void Game::erase(Entity *e)
{
	ERASE(vEntity,ie,e);
	if(e->entity.flags&ENTITY_PROP)
		ERASE(vProps,ip,e);
	if(e->entity.flags&ENTITY_UPDATE)
		ERASE(vUpdate,iu,e);
	if(e->entity.flags&ENTITY_DRAW)
		ERASE(vDraw,id,e);
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
			insert(new PropPlane(x,y,nx,ny));
		}
		else
		if(in=="circle")
		{
			double x, y, r;
			f >> x >> y >> r;
			insert(new PropCircle(x,y,r));
		}
		else
		if(in=="player")
		{
			double x, y;
			f >> x >> y;
			EntPlayer *player = new EntPlayer;
			player->pos = Vector2d(x,y);
			insert(player);
		}
		else
		if(in=="enemy")
		{
			double x, y, d;
			f >> x >> y >> d;
			EntEnemy *enemy = new EntEnemy;
			enemy->pos = Vector2d(x,y);
			enemy->aim = Vector2d(d,0).normalized();
			insert(enemy);
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
	Entity *e;
	double dt = time.delta*time.speed;

	for(int i=0;i<vUpdate.size();i++)
	{
		e = vUpdate[i];
		e->updateLogic(dt);
		if(!(e->entity.flags&ENTITY_ALIVE))
		{
			erase(e);
			delete e;
			i--;
		}
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

void Game::drawLight()
{
	double aspect = (double)window.width/(double)window.height;

	double c = 0.25;
	glClearColor(c,c,c,1);
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();
	glOrtho(-aspect*8.0,aspect*8.0,-8,8,-1,1);
	glTranslated(-game.camera.x,-game.camera.y,0);

	DrawLightCircle(0,0,4,1,0,0);

	/*DrawBlendSubtract();

	double f = sin(glfwGetTime()*0.5);
	f*=f;
	glColor4f(1,1,1,0.25+0.25*f);
	resources.drawSprite2c(5,playerPos+Vector2d(0,0.905),Vector2d(4.0,4.0)*(2.0-0.5*f));

	//glBegin(GL_TRIANGLES);
	//lightCircle(playerPos.x,playerPos.y,5.0,0.25,0.25,0.25);
	//glEnd();

	DrawBlendNormal();*/

	for(int i=0;i<vUpdate.size();i++)
	{
		vUpdate[i]->draw(ENTITY_DRAW_LIGHT);
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

	for(int i=0;i<vDraw.size();i++)
	{
		vDraw[i]->draw(ENTITY_DRAW_COLOR);
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

double Human::updateAuction(double dt)
{
	Collision c;
	collisions.clear();
	for(int i=0;i<game.vProps.size();i++)
	{
		double t = dt;
		if(game.vProps[i]->calcImpactLine(pos,height,vel,t,c) && t<=dt)
		{
			if(t!=dt)
				collisions.clear();
			collisions.push_back(c);
			dt = t;
		}
	}
	return dt;
}

void Human::updatePhysics(double dt)
{
	pos+=vel*dt;
	if(isWinner(dt))
	for(int i=0;i<collisions.size();i++)
	{
		Collision &c = collisions[i];
		pos-=c.p*c.n;
		if(c.v<0.0)
		{
			double dv = (1.0*c.n*vel);
			vel-=dv*c.n;
		}
	}
	collisions.clear();
}

Human::Human()
{
	entity.flags|=ENTITY_HUMAN|ENTITY_UPDATE|ENTITY_DRAW;
	mass = 75.0;
	height = 2.0;
	maxHealth = health = 100.0;
}

Human::~Human()
{
}
