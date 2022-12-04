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
    vector<Particle> particles_mesh;
    double lenghtX, lenghtY;
    int numParticleX, numParticleY;
    int numParticleBullet = 10;
    int rank, size;
    int send_initial_size_tag = 0;
    int send_particle_initial_tag = 1;
    int send_particle_size_calc_tag = 2;
    int send_particle_calc_tag = 3;
    int time = 20;
    double dt = 0.01;
    double startwtime = 0.0;
    double endwtime;
    lenghtX = 10;
    lenghtY = 10;
    numParticleX = 10;
    numParticleY = 10;


    MPI_Status status;
    /* Инициализируем MPI*/
    MPI_Init(&argc, &argv);
    /* Получаем текущий процессор*/
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    /* Получаем общее число процессоров*/
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        /*Генерируем сетку на главном процессоре*/
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
        /*Добавляем пулю*/
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

        /*Объявляем переменную для длины вектора. Далее будем передавать ее другим процессорам*/
        int size_mesh = particles_mesh.size();

        for (int rank_i = 1; rank_i < size; rank_i++) {
            MPI_Send(&size_mesh, 1, MPI_INT, rank_i, send_initial_size_tag, MPI_COMM_WORLD);
            /*Количество передаваемых значений size_send * 7, так как у каждой частицы 7 переменных*/
            MPI_Send(&particles_mesh[0], size_mesh * 7, MPI_DOUBLE, rank_i, send_particle_initial_tag, MPI_COMM_WORLD);
        }
    }
    /*Если процессор не главный, то принимаем с главного процессора сетку*/
    if (rank != 0) {
        int size_mesh_recv;
        MPI_Recv(&size_mesh_recv, 1, MPI_INT, 0, send_initial_size_tag, MPI_COMM_WORLD, &status);
        particles_mesh.resize(size_mesh_recv);
        MPI_Recv(&particles_mesh[0], size_mesh_recv * 7, MPI_DOUBLE, 0, send_particle_initial_tag, MPI_COMM_WORLD, &status);
    }
    /*Считаем минимальный и максмимальный индекс для процессора*/
    int i_min = rank * particles_mesh.size() / size;
    int i_max = (rank + 1) * particles_mesh.size() / size;
    int cnt = 0;

    for (double t = 0; t <= time; t += dt) {
        cnt++;
        ForceCalculate(particles_mesh, i_min, i_max);
        SpeedCalculate(particles_mesh, dt, i_min, i_max);
        CoordinateCalculate(particles_mesh, dt, i_min, i_max);
        //Определяем частицы, которые мы будем отправлять
        vector<Particle> particles_send;
  
        for (int i = i_min; i < i_max; i++) {
            particles_send.push_back(particles_mesh[i]);
        }
        //Посчитанную часть отправляем другим процессорам
        int size_send = particles_send.size();
        for (int rank_i = 0; rank_i < size; rank_i++) {
            if (rank_i != rank) {
                MPI_Send(&size_send, 1, MPI_INT, rank_i, send_particle_size_calc_tag, MPI_COMM_WORLD);
                MPI_Send(&particles_send[0], size_send * 7, MPI_DOUBLE, rank_i, send_particle_calc_tag, MPI_COMM_WORLD);
            }
        }
        //Получаем посчитанную часть
        for (int rank_i = 0; rank_i < size; rank_i++) {
            if (rank_i != rank) {
                vector<Particle> particles_recv;
                int size_recv;
                MPI_Recv(&size_recv, 1, MPI_INT, rank_i, send_particle_size_calc_tag, MPI_COMM_WORLD, &status);
                particles_recv.resize(size_recv);
                MPI_Recv(&particles_recv[0], size_recv * 7, MPI_DOUBLE, rank_i, send_particle_calc_tag, MPI_COMM_WORLD, &status);
                for (int j = 0; j < particles_recv.size(); j++) {
                    particles_send.push_back(particles_recv[j]);
                }
            }
        }

        //На главном процессоре собираем все посчитанные части
        if (rank == 0) {
            //Настраиваем запись в файл
            ofstream myfile;
            string FILE_NAME = ".\\data_plot\\mesh";
            //Записываем в файл только целые значения для времени
            if (cnt % 100 == 0) {
                stringstream stream;
                stream << fixed << setprecision(2) << t;
                string t_str = stream.str();
                string t_new = ReplaceExclamation(t_str);
                string newFileName = FILE_NAME + t_new + ".csv";

                myfile.open(newFileName);

                for (int i = 0; i < particles_send.size(); i++) {
                    myfile << particles_send[i].r.x << "," << particles_send[i].r.y << "\n";
                }

                myfile.close();
        
            }

        }

    }

    if (rank == 0) {
        endwtime = MPI_Wtime();
        cout << "time= " << (endwtime - startwtime) << endl;
    }
 
    MPI_Finalize();
    return 0;
}

