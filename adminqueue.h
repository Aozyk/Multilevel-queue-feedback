#ifndef ADMINQUEUE_H
#define ADMINQUEUE_H

#include <queue>
#include <string>
#include "readyqueue.h"
#include "process.h"

class AdminQ {
    private:
        int num_process = 0; //Si el numero de procesos es 0, el sistema está "dormido".
        const float adminTimeConstraint = 8.0f;
        const float sub1TimeConstraint = 2.0f;
        const float sub2TimeConstraint = 4.0f;
        int currentLevel = 0; //starts at 0.
        std::queue<Process> rQ[4];
        std::vector<Process> tempQ;
        bool running = true;
            /* 
            * 0 = round-robin con q = 2
            * 1 = round-robin con q = 4
            * 2 = priority apropiativo
            * 3 = FCFS
            */
        float adminTime; //Tiempo actual de la admin queue
        float subTime; // Tiempo actual de la "sub" cola. (necesario para el round robin de las colas)

        int checkActiveQ(); //Checke las colas para ver cual es la de menor nivel con procesos

        void changeLevel(); // Cambiar de colas
        void resetTimers(); 
        void resetSubTimer(); 

        void flushToQueue(int Lvl); // Mueve todos los elementos del vector temporal a la cola con el nivel "Lvl"
        void flushToVector(int Lvl); // Mover todos los elementos de la cola de nivel "LVL" al vector temporal

        bool checkAdminTime(); // Retorna true si está al limite de tiempo
        bool checkSubTime(); // Retorna true si está al limite de tiempo

        std::string debugLvlQ(int Lvl);
        std::string debugTempVector();
        std::string getProcessList(int Lvl);

    public:
        AdminQ();
        ~AdminQ(); //dtor

        void handleInput(float inArrival);      //funcion que handle input como simio. unga unga
        void updateProcess(float time);
        bool isRunning() { return running; }

        void debug(); //print

};

#endif