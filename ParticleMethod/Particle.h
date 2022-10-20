#pragma once
#include "Vector2d.h";
#include <vector>
using namespace std;
#include <math.h>;

class Particle
{
public:

	double m = 1;
	Vector2d summaryForce;
	Vector2d r, v;

};

double Force(Vector2d r1, Vector2d r2);

void ForceCalculated(vector<Particle> particles);
void SpeedCalculated(vector<Particle> particles, double dt);
void CoordinateCalculated(vector<Particle> particles, double dt);
