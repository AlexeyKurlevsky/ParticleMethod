#include "Particle.h"

double Force(Vector2d r1, Vector2d r2) {

	double c = 100, d = 1;

	double F = c * ((r1 - r2).length() - d);

	return F;
}
