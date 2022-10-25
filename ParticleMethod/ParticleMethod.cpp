﻿// ParticleMethod.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "Particle.h"
#include <fstream>

using namespace std;

//ofstream outputFile;
//ofstream fs;
//std::string filename = "test.csv";

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
    fstream fout;
    fout.open("test.csv");

    for (int t = 0; t < time; t++) {
        ForceCalculated(particles);
        SpeedCalculated(particles, dt);
        CoordinateCalculated(particles, dt);


        fout << particles[0].r.x << "," << particles[0].r.y << "," << particles[1].r.x << "," << particles[1].r.y << std::endl;

        cout << "Coordinate a1.x= " << particles[0].r.x << endl;
        cout << "Coordinate a1.y= " << particles[0].r.y << endl;
        cout << "Coordinate a2.x= " << particles[1].r.x << endl;
        cout << "Coordinate a2.y= " << particles[1].r.y << endl;
        cout << "Force = " << F << endl;

    }
}

