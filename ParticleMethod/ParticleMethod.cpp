// ParticleMethod.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "Particle.h"


int main()
{
    Particle a1, a2;
    a1.r = Vector2d(0, 0);
    a2.r = Vector2d(1, 0);

    double F = Force(a1.r, a2.r);

    std::cout << F;
}

