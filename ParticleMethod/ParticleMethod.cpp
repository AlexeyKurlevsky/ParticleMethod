// ParticleMethod.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "Particle.h"
#include <fstream>

using namespace std;

int main()
{
    Particle a1, a2, a3, a4;
    double lenghtX, lenghtY, temp_x=0, temp_y=0;
    int numParticleX, numParticleY;
    lenghtX = 10;
    lenghtY = 10;
    numParticleX = 10;
    numParticleY = 10;

    a1.r = Vector2d(0, 0);
    a2.r = Vector2d(1.1, 0);

    vector<Particle> particles_mesh;

    for (int i = 0; i < numParticleX; i++) {
        double coordX = i * lenghtX / numParticleX;
        for (int j = 0; j < numParticleY; j++) {
            if (i % 2 == 0) {
                double coordY = j * lenghtY / numParticleY;
                a3.r = Vector2d(coordX, coordY);
                particles_mesh.push_back(a3);
            }
            else {
                double coordY = j * lenghtY / numParticleY +  lenghtY / numParticleY / 2;
                a3.r = Vector2d(coordX, coordY);
                particles_mesh.push_back(a3);
            }
        }
    }
    
    int numParticleBullet = 100;

    for (int i = 0; i < numParticleBullet; i++) {
        double rand_x = -1+2*(float)rand() / RAND_MAX;
        double rand_y = -1+2*(float)rand() / RAND_MAX;
        double origin_dist = rand_x * rand_x + rand_y * rand_y;
        if (origin_dist <= 1)
            a4.r = Vector2d(rand_x+ lenghtX*1.5, rand_y+ lenghtY/2);
            a4.v= Vector2d(1, 0);
            particles_mesh.push_back(a4);
    }

    int time = 1000;
    double dt = 0.0001;

    std::ofstream myfile;
    myfile.open("D:\\plot\\mesh.csv");

    for (int i = 0; i < particles_mesh.size(); i++) {
        myfile << particles_mesh[i].r.x << "," << particles_mesh[i].r.y << "\n";
    }
    myfile.close();

    myfile.open("D:\\plot\\res.csv");

    for (int t = 0; t < time; t++) {
        ForceCalculate(particles_mesh);
        SpeedCalculate(particles_mesh, dt);
        CoordinateCalculate(particles_mesh, dt);
        
        if (t == 200) {
            for (int i = 0; i < particles_mesh.size(); i++) {
                myfile << particles_mesh[i].r.x << "," << particles_mesh[i].r.y << "\n";
            }
            myfile.close();
        }
      
    }

    return 0;
}

