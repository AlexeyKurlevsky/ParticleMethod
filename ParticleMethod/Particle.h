#pragma once
#include "Vector2d.h";

class Particle
{
public:

	int m = 1;
	Vector2d r, v;


};

double Force(Vector2d r1, Vector2d r2);

