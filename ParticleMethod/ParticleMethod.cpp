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
    vector<Particle> particles_calc_recv;
    double lenghtX, lenghtY;
    int numParticleX, numParticleY;
    int numParticleBullet = 10;
    int rank, size;
    int send_size_tag = 0;
    int send_particle_calc_tag = 1;
    int send_particle_ready_tag = 2;
    int time = 20;
    double dt = 0.01;
    double startwtime = 0.0;
    double endwtime;
    lenghtX = 10;
    lenghtY = 10;
    numParticleX = 10;
    numParticleY = 10;


    MPI_Status status;
    // Инициализируем MPI
    MPI_Init(&argc, &argv);
    // Получаем текущий процессор
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // Получаем общее число процессоров
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        vector<Particle> particles_mesh;
        //Генерируем сетку на главном процессоре
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
        //Добавляем пулю
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
        //Считаем минимальный и максмимальный индекс для главного процессора
        int i_min = rank * particles_mesh.size() / size;
        int i_max = (rank + 1) * particles_mesh.size() / size;
        //Объявляем переменную для длины вектора. Далее будем передавать ее другим процессорам
        int size_send = particles_mesh.size();
        //Настраиваем запись в файл
        ofstream myfile;
        string FILE_NAME = ".\\data_plot\\mesh";
        int cnt = 0;

        for (double t = 0; t <= time; t += dt) {
            cnt++;
            //Считаем часть предназначенную для главного процессора
            ForceCalculate(particles_mesh, i_min, i_max);
            SpeedCalculate(particles_mesh, dt, i_min, i_max);
            CoordinateCalculate(particles_mesh, dt, i_min, i_max);
            //Передаем посчитанные значения другим процессорам
            for (int rank_i = 1; rank_i < size; rank_i++) {
                MPI_Send(&size_send, 1, MPI_INT, rank_i, send_size_tag, MPI_COMM_WORLD);
                //Количество передаваемых значений size_send * 7, так как у каждой частицы 7 переменных
                MPI_Send(&particles_mesh[0], size_send*7, MPI_DOUBLE, rank_i, send_particle_calc_tag, MPI_COMM_WORLD);
            }
            //Получаем результат с последнего процессора
            MPI_Recv(&particles_mesh[0], size_send*7, MPI_DOUBLE, size - 1, send_particle_ready_tag, MPI_COMM_WORLD, &status);
            //Записываем в файл только целые значения для времени
            if (cnt % 100 == 0) {
                stringstream stream;
                stream << fixed << setprecision(2) << t;
                string t_str = stream.str();
                string t_new = ReplaceExclamation(t_str);
                string newFileName = FILE_NAME + t_new + ".csv";

                myfile.open(newFileName);

                for (int i = 0; i < particles_mesh.size(); i++) {
                    myfile << particles_mesh[i].r.x << "," << particles_mesh[i].r.y << "\n";
                }

                myfile.close();

            }

        }

        endwtime = MPI_Wtime();
        cout << "time= " << (endwtime - startwtime) * 1000 << endl;

    }
    else {
        //Принимаем размер вектора, который мы будем получать с главного процессора.
        int size_calc;
        MPI_Recv(&size_calc, 1, MPI_INT, 0, send_size_tag, MPI_COMM_WORLD, &status);
        //У вектора, в который будем записывать принимаемые значения, меняем длину. Иначе не получится корректно принять
        particles_calc_recv.resize(size_calc);

        int i_min = rank * size_calc / size;
        int i_max = (rank + 1) * size_calc / size;
        //Проводим расчет для частиц, выделенных на конкретном процессоре
        for (double t = 0; t <= time; t += dt) {
            MPI_Recv(&particles_calc_recv[0], size_calc * 7, MPI_DOUBLE, 0, send_particle_calc_tag, MPI_COMM_WORLD, &status);
            ForceCalculate(particles_calc_recv, i_min, i_max);
            SpeedCalculate(particles_calc_recv, dt, i_min, i_max);
            CoordinateCalculate(particles_calc_recv, dt, i_min, i_max);
            //Отправляем результат на главный процессор
            MPI_Send(&particles_calc_recv[0], size_calc * 7, MPI_DOUBLE, 0, send_particle_ready_tag, MPI_COMM_WORLD);
        }

    }
 
    MPI_Finalize();
    return 0;
}

