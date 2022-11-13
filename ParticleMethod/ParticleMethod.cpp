// ParticleMethod.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "Particle.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

string ReplaceExclamation(string text)
{
    string newText = text;
    int i, len = text.size();

    for (i = 0; i < len + 1; i++)
    {

        if (text[i] == '.')
            newText[i] = '_';
    }
    return newText;
}

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

    int numParticleBullet = 10;
    a4.r = Vector2d(lenghtX * 1.5, lenghtY / 2);
    a4.v = Vector2d(-1, 0);
    particles_mesh.push_back(a4);
    for (int i = 1; i < numParticleBullet; i++) {
        double coordX = i * lenghtX / numParticleX;
        double coordY = lenghtY / numParticleY;
        a4.r = Vector2d(lenghtX*1.5+ coordX, coordY+lenghtY/2);
        a4.v = Vector2d(-1, 0);
        particles_mesh.push_back(a4);
        a4.r = Vector2d(lenghtX * 1.5 + coordX, lenghtY / 2 - coordY);
        a4.v = Vector2d(-1, 0);
        particles_mesh.push_back(a4);
        a4.r = Vector2d(lenghtX * 1.5 + coordX, lenghtY / 2);
        a4.v = Vector2d(-1, 0);
        particles_mesh.push_back(a4);
    }

    int time = 20;
    double dt = 0.01;

    ofstream myfile;
    string FILE_NAME = ".\\data\\mesh";

    for (double t = 0; t <= time; t += dt) {
        ForceCalculate(particles_mesh);
        SpeedCalculate(particles_mesh, dt);
        CoordinateCalculate(particles_mesh, dt);

        stringstream stream;
        stream << fixed << setprecision(2) << t;
        string t_str = stream.str();
        string t_new = ReplaceExclamation(t_str);
        string newFileName = FILE_NAME + t_new + ".csv";
   /*     cout << newFileName << endl;*/
        
        myfile.open(newFileName);

        for (int i=0; i < particles_mesh.size(); i++) {
            myfile << particles_mesh[i].r.x << "," << particles_mesh[i].r.y << "\n";
        }

        myfile.close();
    }
 

    return 0;
}

