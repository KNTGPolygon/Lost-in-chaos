#include "Window.h"
#include "Props.h"

void PropWall::draw()
{
	Vector2d n = 1024.0*normal;
	glColor4f(color.x,color.y,color.z,1);
	glBegin(GL_TRIANGLES);
	glVertex2f(pos.x-n.y,pos.y+n.x);
	glVertex2f(pos.x+n.y,pos.y-n.x);
	glVertex2f(pos.x-n.x,pos.y-n.y);
	glEnd();
}

bool PropWall::calcImpact(Vector2d p, Vector2d v, double &time, Collision &c)
{
	double h = (p-pos)*normal;
	double vr = v*normal;
	if(h<=0.0 && ((IsZero(h) && vr<0.0 && !IsZero(vr)) || !IsZero(h)))
	{
		time = 0.0;
		c.n = normal;
		c.p = h;
		c.v = (vr<0.0 ? vr : 0.0);
		return true;
	}
	else
	if(h>0.0 && vr<0.0)
	{
		time = -h/vr;
		c.n = normal;
		c.p = 0.0;
		c.v = vr;
		return true;
	}
	return false;
}

PropWall::PropWall(double x, double y, double w, double h) : pos(x,y), normal(w,h)
{
	color = Vector3d(1,1,1);
	normal.normalize();
}

PropWall::PropWall()
{
	pos = Vector2d(0,0);
	normal = Vector2d(0,1);
	color = Vector3d(1,1,1);
}

PropWall::~PropWall()
{
}

