#include "Window.h"
#include "Props.h"

using namespace std;

bool Prop::calcImpact(Vector2d p, Vector2d v, double &time, Collision &c)
{
	return false;
}

void Prop::draw()
{
}

Prop::Prop()
{
	propType = PROP_NONE;
}

Prop::~Prop()
{
}

bool PropPlane::calcImpact(Vector2d p, Vector2d v, double &time, Collision &c)
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

void PropPlane::draw()
{
	Vector2d n = 1024.0*normal;
	glColor4f(color.x,color.y,color.z,1);
	glBegin(GL_TRIANGLES);
	glVertex2f(pos.x-n.y,pos.y+n.x);
	glVertex2f(pos.x+n.y,pos.y-n.x);
	glVertex2f(pos.x-n.x,pos.y-n.y);
	glEnd();
}

PropPlane::PropPlane(double x, double y, double w, double h) : normal(w,h)
{
	pos.x = x, pos.y = y;
	color = Vector3d(1,1,1);
	normal.normalize();
	if(normal.magnitude2()<0.5)
		normal = Vector2d(0,1);
}

PropPlane::PropPlane()
{
	pos = Vector2d(0,0);
	normal = Vector2d(0,1);
	color = Vector3d(1,1,1);
}

PropPlane::~PropPlane()
{
}

bool PropCircle::calcImpact(Vector2d p, Vector2d v, double &time, Collision &col)
{
	double d = (p-pos).magnitude2()-radius*radius;
	if(d<=0.0)
	{
		time = 0.0;
		col.n = (p-pos).normalized();
		if(col.n.magnitude2()<0.5)
			col.n = Vector2d(0,1);
		col.p = d;
		double vr = v*col.n;
		col.v = (vr<0.0 ? vr : 0.0);
		return true;
	}
	else
	if(d>0.0)
	{
		double a = v.magnitude2();
		double b = 2.0*(p-pos)*v;
		double c = -radius*radius;
		double delta = b*b-4.0*a*c;
		if(delta<0.0)
			return false;
		delta = sqrt(delta);
		double t1 = (-b-delta)/(2.0*a);
		double t2 = (-b+delta)/(2.0*a);
		if(t1<0.0 && t2<0.0)
			return false;
		time = (t1<0.0 ? t2 : (t2<0.0 ? t1 : min(t1,t2)));
		col.n = (p-pos+v*time).normalized();
		if(col.n.magnitude2()<0.5)
			col.n = Vector2d(0,1);
		col.p = 0.0;
		col.v = v*col.n;
		return true;
	}
	return false;
}

void PropCircle::draw()
{
	glColor4f(1,1,1,1);
	glBegin(GL_TRIANGLES);
	const int n = 32;
	for(int i=0;i<n;i++)
	{
		double a = 2.0*PI*(double)i/(double)n;
		double b = 2.0*PI*(double)(i+1)/(double)n;
		glVertex2f(pos.x,pos.y);
		glVertex2f(pos.x+radius*cos(a),pos.y+radius*sin(a));
		glVertex2f(pos.x+radius*cos(b),pos.y+radius*sin(b));
	}
	glEnd();
}

PropCircle::PropCircle(double x, double y, double r) : radius(r)
{
	pos.x = x, pos.y = y;
}

PropCircle::PropCircle()
{
	radius = 1.0;
}

PropCircle::~PropCircle()
{
}