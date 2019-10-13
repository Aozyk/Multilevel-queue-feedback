#include <iostream>
#include <chrono>
#include <conio.h>
#include <windows.h>
#include "adminqueue.h"
#include <iomanip>

/**
    Trabajo sistema operativos 14/10 "Multiqueue level feedback"
    Integrantes: Daniel Aguilera
                 Nicolás Andrews
                 Iván Perez
    Docente: Jorge Morris
    Código asignatura: INFBALGO
    Sección: 411? 1? no sé

    Compiler flags: -std=gnu++11
*/

using namespace std::chrono;

high_resolution_clock::time_point time1;
high_resolution_clock::time_point time2;
duration<float, std::milli> deltaTime;

using namespace std;

int main() {
    AdminQ aq;
    float acum = 0.f;
    time1 = high_resolution_clock::now();
    time2 = high_resolution_clock::now();
    deltaTime = time2 - time1;

    while(aq.isRunning()) { //main loop
        cout << "< Aprete cualquier tecla para agregar un nuevo proceso >\n\n";
        if(_kbhit()) {
            aq.handleInput(acum);
        }
        time1 = high_resolution_clock::now();
        aq.updateProcess(deltaTime.count()/100.f);
        aq.debug();
        time2 = high_resolution_clock::now();
        deltaTime = time2 - time1;
        acum += deltaTime.count()/100.f;
        cout << fixed <<setprecision(2);
        cout << "Cronometro: " << acum;

        system("cls");
    }

    return 0;
}