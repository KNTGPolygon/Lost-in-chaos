#pragma once

#include "Math.h"

inline Vector2d Friction(Vector2d v, Vector2d F, Vector2d n)
{
	Vector2d t = v-(n*(v*n));
	if(IsZero(F*n) || abs(v*n)>0.01 || IsZero(t.magnitude2()))
		return Vector2d(0.0,0.0);
	return (F*n)*(t.normalized());
}
