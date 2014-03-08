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
	if(vr<0.0)
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

// PropRectangle

inline int findMax4(double *t)
{
	int winner = 0;
	for(int i=1;i<4;i++)
	if(t[i]>t[winner])
		winner = i;
	return winner;
}

bool PropRectangle::calcImpact(Vector2d p, Vector2d v, double &time, Collision &c)
{
	static Vector2d n[] = {Vector2d(-1,0),Vector2d(1,0),Vector2d(0,-1),Vector2d(0,1)};
	static int other0[] = {2,2,0,0};
	static int other1[] = {3,3,1,1};

	double d[] = {width*0.5,width*0.5,height*0.5,height*0.5};
	double h[4];
	double vr, t, vp;

	for(int i=0;i<4;i++)
		h[i] = (p-pos-((d[i])*n[i]))*n[i];

	if(h[0]<=0 && h[1]<=0 && h[2]<=0 && h[3]<=0)
	{
		int w = findMax4(h);
		vr = n[w]*v;
		if(IsZero(h[w]) && (vr>0.0 || IsZero(vr)))
			return false;
		time = 0.0;
		c.n = n[w];
		c.p = h[w];
		c.v = (vr<0.0 ? vr : 0.0);
		return true;
	}
	else
	for(int i=0;i<4;i++)
	if(h[i]>0.0)
	{
		vr = v*n[i];
		vp = (v-vr*n[i])*n[other0[i]];
		t = -h[i]/vr;
		if(vr<0 && h[other0[i]]+vp*t<=0 && h[other1[i]]-vp*t<=0)
		{
			time = t;
			c.n = n[i];
			c.p = 0.0;
			c.v = vr;
			return true;
		}
	}

	return false;
}

bool PropRectangle::calcImpactLine(Vector2d p, double height, Vector2d v, double &time, Collision &c)
{
	return calcImpact(p,v,time,c);
}

bool PropRectangle::onEdge(Vector2d p, Vector2d &normal)
{
	static Vector2d n[] = {Vector2d(-1,0),Vector2d(1,0),Vector2d(0,-1),Vector2d(0,1)};

	double d[] = {width*0.5,width*0.5,height*0.5,height*0.5};
	double h[4];

	for(int i=0;i<4;i++)
		h[i] = (p-pos-((d[i])*n[i]))*n[i];
	if(h[0]<PROP_EDGE_HEIGHT && h[1]<PROP_EDGE_HEIGHT && h[2]<PROP_EDGE_HEIGHT && h[3]<PROP_EDGE_HEIGHT)
	{
		normal = n[findMax4(h)];
		return true;
	}
	return false;
}

void PropRectangle::draw(int mode)
{
	if(mode!=ENTITY_DRAW_COLOR)
		return;
	glBindTexture (GL_TEXTURE_2D, resources.texture[4]);

	double x[] = {pos.x-width*0.5,pos.x-width*0.5,pos.x+width*0.5,pos.x+width*0.5};
	double y[] = {pos.y-height*0.5,pos.y+height*0.5,pos.y+height*0.5,pos.y-height*0.5};

	glColor4f(1,1,1,1);
	glBegin(GL_QUADS);
	for(int i=0;i<4;i++)
	{
		glTexCoord2f(x[i]*0.5,y[i]*0.5);
		glVertex2f(x[i],y[i]);
	}
	glEnd();
}

PropRectangle::PropRectangle(double x, double y, double w, double h)
{
	pos.x = x, pos.y = y;
	width = w, height = h;
}

PropRectangle::PropRectangle()
{
	pos = Vector2d(0,0);
	width = height = 1.0;
}

PropRectangle::~PropRectangle()
{
}
