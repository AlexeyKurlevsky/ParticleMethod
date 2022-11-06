// ParticleMethod.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
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
    Particle a1, a2, a3;
    double lenghtX, lenghtY, temp_x=0, temp_y=0;
    int numParticleX, numParticleY;
    lenghtX = 1;
    lenghtY = 5;
    numParticleX = 10;
    numParticleY = 11;

    a1.r = Vector2d(0, 0);
    a2.r = Vector2d(1.1, 0);

    vector<Particle> particles_mesh;
    particles_mesh.push_back(a3);
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

    cout << particles_mesh.size() << endl;
    vector<Particle> particles;
    particles.push_back(a1);
    particles.push_back(a2);

    double F = Force(a1.r, a2.r);
    int time = 1000;
    double dt = 0.01;

    std::ofstream myfile;
    myfile.open("D:\\plot\\mesh.csv");

    for (int i = 0; i < particles_mesh.size(); i++) {
        myfile << particles_mesh[i].r.x << "," << particles_mesh[i].r.y << "\n";
    }


    for (int t = 0; t < time; t++) {
        ForceCalculate(particles);
        SpeedCalculate(particles, dt);
        CoordinateCalculate(particles, dt);

        /* cout << "Coordinate a1.r.x " << particles[0].r.x << endl;
         cout << "Coordinate a1.r.y " << particles[0].r.y << endl;
         cout << "Coordinate a2.r.x " << particles[1].r.x << endl;
         cout << "Coordinate a2.r.y " << particles[1].r.y << endl;
         cout << "Force = " << F << endl;
 */
        //myfile << particles[0].r.x << "," << particles[0].r.y << "," << particles[1].r.x << "," << particles[1].r.y << "\n";

    }

    //myfile.close();
    return 0;
}

