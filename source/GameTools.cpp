#include "Game.h"
#include "Props.h"

using namespace std;

bool Game::findGround(Vector2d pos, Vector2d vel, Vector2d &outn)
{
	Vector2d n;
	double k;
	bool any = false;
	for(int i=0;i<vProps.size();i++)
	if(vProps[i]->onEdge(pos,n) && (!any || k>vel*n))
		outn+=n, k = vel*n, any = true;
	return any;
}
