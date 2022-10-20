#include "Particle.h"

double Force(Vector2d r1, Vector2d r2) {

	double c = 100, d = 1;
	double F = c * ((r1 - r2).length() - d);
	return F;
}

void ForceCalculated(vector<Particle> particles) {
	for (int i = 0; i < particles.size(); i++) {
		particles[i].summaryForce.resetToZero();
		for (int j = 0; j < particles.size(); j++) {
			if (i != j) {
				Vector2d dif = Vector2d(particles[i].r - particles[j].r);
				double difNorm = dif.length();
				if (dif.length() < sqrt(2)) {
					double F = Force(Vector2d(particles[i].r), Vector2d(particles[j].r));
					particles[i].summaryForce = particles[i].summaryForce + F * (1 / difNorm) * dif;
				}
			}
		}
	}

}

void SpeedCalculated(vector<Particle> particles, double dt) {
	for (int i = 0; i < particles.size(); i++) {
		particles[i].v = particles[i].v + dt * particles[i].summaryForce / particles[i].m;
	}
}

void CoordinateCalculated(vector<Particle> particles, double dt) {
	for (int i = 0; i < particles.size(); i++) {
		particles[i].r = particles[i].r + dt * particles[i].summaryForce;
	}
}
