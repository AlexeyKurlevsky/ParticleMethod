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

	Particle() {
		r.resetToZero();
		v.resetToZero();
		summaryForce.resetToZero();
	}

};

double Force(Vector2d r1, Vector2d r2);
void ForceCalculate(vector<Particle>& particles);
void SpeedCalculate(vector<Particle>& particles, double dt);
void CoordinateCalculate(vector<Particle>& particles, double dt);
