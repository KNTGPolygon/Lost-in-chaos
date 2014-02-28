#include "Window.h"
#include "Props.h"
#include "Resources.h"

using namespace std;

bool Prop::calcImpact(Vector2d p, Vector2d v, double &time, Collision &c)
{
	return false;
}

bool Prop::calcImpactLine(Vector2d p, double height, Vector2d v, double &time, Collision &c)
{
	return false;
}

bool Prop::onEdge(Vector2d p, Vector2d &n)
{
	return false;
}

Prop::Prop() : Entity()
{
	propType = PROP_NONE;
	entity.flags|=ENTITY_PROP|ENTITY_DRAW;
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

bool PropPlane::calcImpactLine(Vector2d p, double height, Vector2d v, double &time, Collision &c)
{
	double h1 = (p-pos)*normal;
	double h2 = (p-pos+Vector2d(0,height))*normal;
	double vr = v*normal;
	if(h1<=0.0 && ((IsZero(h1) && vr<0.0 && !IsZero(vr)) || !IsZero(h1)))
	{
		time = 0.0;
		c.n = normal;
		c.p = h1;
		c.v = (vr<0.0 ? vr : 0.0);
		return true;
	}
	else
	if(h2<=0.0 && ((IsZero(h2) && vr<0.0 && !IsZero(vr)) || !IsZero(h2)))
	{
		time = 0.0;
		c.n = normal;
		c.p = h2;
		c.v = (vr<0.0 ? vr : 0.0);
		return true;
	}
	else
	if(vr<0.0)
	{
		time = -max(h1,h2)/vr;
		c.n = normal;
		c.p = 0.0;
		c.v = vr;
		return true;
	}
	return false;
}

bool PropPlane::onEdge(Vector2d p, Vector2d &n)
{
	n = normal;
	return abs((p-pos)*normal)<PROP_EDGE_HEIGHT;
}

void PropPlane::draw(int mode)
{
	if(mode!=ENTITY_DRAW_COLOR)
		return;
	Vector2d n = 1024.0*normal;

	glBindTexture (GL_TEXTURE_2D, resources.texture[4]);

	double x[] = {pos.x-n.y,pos.x+n.y,pos.x-n.x};
	double y[] = {pos.y+n.x,pos.y-n.x,pos.y-n.y};

	glColor4f(color.x,color.y,color.z,1);
	glBegin(GL_TRIANGLES);
	for(int i=0;i<3;i++)
	{
		glTexCoord2f(x[i]*0.5,y[i]*0.5);
		glVertex2f(x[i],y[i]);
	}
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
	Vector2d n = (p-pos).normalized();
	double vr = v*n;
	if(d<=0.0 && ((IsZero(d) && vr<0.0 && !IsZero(vr)) || !IsZero(d)))
	{
		time = 0.0;
		col.n = n;
		if(col.n.magnitude2()<0.5)
			col.n = Vector2d(0,1);
		col.p = d;
		col.v = (vr<0.0 ? vr : 0.0);
		return true;
	}
	else
	if(d>0.0)
	{
		double a = v.magnitude2();
		double b = 2.0*(p-pos)*v;
		double c = (p-pos).magnitude2()-radius*radius;
		double delta = b*b-4.0*a*c;
		if(delta<0.0)
			return false;
		delta = sqrt(delta);
		double t1 = (-b-delta)/(2.0*a);
		double t2 = (-b+delta)/(2.0*a);
		if(t1<0.0 && t2<0.0)
			return false;
		time = (t1<0.0 ? t2 : (t2<0.0 ? t1 : min(t1,t2)));
		col.n = ((p-pos+v*time).normalized());
		if(col.n.magnitude2()<0.5)
			col.n = Vector2d(0,1);
		col.p = 0.0;
		col.v = v*col.n;
		return (col.v<0.0 && !IsZero(col.v));
	}
	return false;
}

bool PropCircle::calcImpactLine(Vector2d p, double height, Vector2d v, double &time, Collision &col)
{
	return calcImpact(p,v,time,col);
}

bool PropCircle::onEdge(Vector2d p, Vector2d &n)
{
	n = (p-pos).normalized();
	return abs((p-pos).magnitude()-radius)<PROP_EDGE_HEIGHT;
}

void PropCircle::draw(int mode)
{
	if(mode!=ENTITY_DRAW_COLOR)
		return;
	double a, b, xa, ya, xb, yb;
	int n = 32+int(radius);

	glBindTexture (GL_TEXTURE_2D, resources.texture[3]);
	
	glColor4f(1,1,1,1);
	
	glBegin(GL_TRIANGLES);
	for(int i=0;i<n;i++)
	{
		a = 2.0*PI*(double)i/(double)n;
		b = 2.0*PI*(double)(i+1)/(double)n;
		xa = pos.x+radius*cos(a), ya = pos.y+radius*sin(a);
		xb = pos.x+radius*cos(b), yb = pos.y+radius*sin(b);
		glTexCoord2f(pos.x*0.5,pos.y*0.5);
		glVertex2f(pos.x,pos.y);
		glTexCoord2f(xa*0.5,ya*0.5);
		glVertex2f(xa,ya);
		glTexCoord2f(xb*0.5,yb*0.5);
		glVertex2f(xb,yb);
	}
	glEnd();
}

PropCircle::PropCircle(double x, double y, double r) : Prop(), radius(r)
{
	propType = PROP_CIRCLE;
	pos.x = x, pos.y = y;
}

PropCircle::PropCircle()
{
	radius = 1.0;
}

PropCircle::~PropCircle()
{
}
