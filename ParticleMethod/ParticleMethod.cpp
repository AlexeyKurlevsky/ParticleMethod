// ParticleMethod.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <mpi.h>
#include <stdio.h>
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

int main(int argc, char* argv[])
{   
    Particle a3, a4;
    double lenghtX, lenghtY;
    int numParticleX, numParticleY;
    int numParticleBullet = 1;
    int rank, size;
    int send_size_tag = 0;
    int send_particle_calc_tag = 1;
    int send_particle_ready_tag = 2;
    int time = 20;
    double dt = 0.01;
    double startwtime = 0.0;
    double endwtime;
    lenghtX = 3;
    lenghtY = 3;
    numParticleX = 3;
    numParticleY = 3;


    MPI_Status status;
    // Инициализируем MPI
    MPI_Init(&argc, &argv);
    // Получаем текущий процессор
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // Получаем общее число процессоров
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        vector<Particle> particles_main;
        vector<Particle> particles_mesh;
        startwtime = MPI_Wtime();
        for (int i = 0; i < numParticleX; i++) {
            double coordX = i * lenghtX / numParticleX;
            for (int j = 0; j < numParticleY; j++) {
                if (i % 2 == 0) {
                    double coordY = j * lenghtY / numParticleY;
                    a3.r = Vector2d(coordX, coordY);
                    particles_mesh.push_back(a3);
                }
                else {
                    double coordY = j * lenghtY / numParticleY + lenghtY / numParticleY / 2;
                    a3.r = Vector2d(coordX, coordY);
                    particles_mesh.push_back(a3);
                }
            }
        }

        a4.r = Vector2d(lenghtX * 1.5, lenghtY / 2);
        a4.v = Vector2d(-1, 0);
        particles_mesh.push_back(a4);
        for (int i = 1; i < numParticleBullet; i++) {
            double coordX = i * lenghtX / numParticleX;
            double coordY = lenghtY / numParticleY;
            a4.r = Vector2d(lenghtX * 1.5 + coordX, coordY + lenghtY / 2);
            a4.v = Vector2d(-1, 0);
            particles_mesh.push_back(a4);
            a4.r = Vector2d(lenghtX * 1.5 + coordX, lenghtY / 2 - coordY);
            a4.v = Vector2d(-1, 0);
            particles_mesh.push_back(a4);
            a4.r = Vector2d(lenghtX * 1.5 + coordX, lenghtY / 2);
            a4.v = Vector2d(-1, 0);
            particles_mesh.push_back(a4);
        }
        cout << "size mesh = " << particles_mesh.size() << endl;
        //Выделяем частицы для главного процессора
        for (int i = rank * particles_mesh.size() / size; i < (rank + 1) * particles_mesh.size() / size; i++) {
            particles_main.push_back(particles_mesh[i]);
        }
        
        for (double t = 0; t <= time; t += dt) {
            ForceCalculate(particles_main);
            SpeedCalculate(particles_main, dt);
            CoordinateCalculate(particles_main, dt);
        }

        for (int rank_i = 1; rank_i < size; rank_i++) {
            //Выделяем частицы для остальных процессоров
            vector<Particle> particles_calc;
            vector<Particle> particles_ready;
            for (int j = rank_i * particles_mesh.size() / size; j < (rank_i + 1) * particles_mesh.size() / size; j++) {
                particles_calc.push_back(particles_mesh[j]);
            }
            int size_send = particles_calc.size();
            //Отправляем размер массива
            MPI_Send(&size_send, 1, MPI_INT, rank_i, send_size_tag, MPI_COMM_WORLD);
            //Отправляем массив для рассчета
            MPI_Send(&particles_calc[0], particles_calc.size(), MPI_DOUBLE, rank_i, send_particle_calc_tag, MPI_COMM_WORLD);
            //Меняем размер массива. Особенности передачи массивов в MPI
            particles_ready.resize(size_send);
            MPI_Recv(&particles_ready[0], size_send, MPI_DOUBLE, rank_i, send_particle_ready_tag, MPI_COMM_WORLD, &status);
            for (int i = 0; i < particles_ready.size(); i++) {
                particles_main.push_back(particles_ready[i]);
            }
        }
        //TODO не работает запись файла
        ofstream myfile;
        string FILE_NAME = ".\\data\\mesh_main.csv";

        myfile.open(FILE_NAME);

        for (int i = 0; i < particles_main.size(); i++) {
            myfile << particles_main[i].r.x << "," << particles_main[i].r.y << "\n";
        }

        myfile.close();

        cout << "size main = " << particles_main.size() << endl;
        endwtime = MPI_Wtime();
        cout << "time= " << (endwtime - startwtime) * 1000 << endl;

    }
    else {
        int size_calc;
        MPI_Recv(&size_calc, 1, MPI_INT, 0, send_size_tag, MPI_COMM_WORLD, &status);
        vector<Particle> particles_calc_recv;
        particles_calc_recv.resize(size_calc);
        MPI_Recv(&particles_calc_recv[0], size_calc, MPI_DOUBLE, 0, send_particle_calc_tag, MPI_COMM_WORLD, &status);

         for (double t = 0; t <= time; t += dt) {
             ForceCalculate(particles_calc_recv);
             SpeedCalculate(particles_calc_recv, dt);
             CoordinateCalculate(particles_calc_recv, dt);
         }

        MPI_Send(&particles_calc_recv[0], particles_calc_recv.size(), MPI_DOUBLE, 0, send_particle_ready_tag, MPI_COMM_WORLD);
    }
 
    MPI_Finalize();
    return 0;
}

