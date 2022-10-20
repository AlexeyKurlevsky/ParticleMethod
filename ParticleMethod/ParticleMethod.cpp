// ParticleMethod.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "Particle.h"
using namespace std;

int main()
{
    Particle a1, a2;
    a1.r = Vector2d(0, 0);
    a2.r = Vector2d(1.1, 0);
    vector<Particle> particles;
    particles.push_back(a1);
    particles.push_back(a2);

    double F = Force(a1.r, a2.r);
    int time = 1000;
    double dt = 0.01;

    for (int t = 0; t < time; t++) {
        ForceCalculated(particles);
        SpeedCalculated(particles, dt);
        CoordinateCalculated(particles, dt);

        cout << "Coordinate a1.x= " << a1.r.x << endl;
        cout << "Coordinate a1.y= " << a1.r.y << endl;
        cout << "Coordinate a2.x= " << a2.r.x << endl;
        cout << "Coordinate a2.y= " << a2.r.y << endl;

    }
}

