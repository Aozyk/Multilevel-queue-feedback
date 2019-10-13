#include "process.h"
#include <iostream>

Process::Process() { //No se debería acceder este método, por lo tanto el ID es -1, es decir, el programa solo aceptará IDs positivos.
    ID = -1;
    turntime = 0;
    arrival_time = 0;
    timeleft = 0;
    priority = 10;
}

Process::Process(const Process& _rhs) {
    ID = _rhs.ID;
    turntime = _rhs.turntime;
    arrival_time = _rhs.arrival_time;
    timeleft = _rhs.timeleft;
    priority = _rhs.priority;
}

Process::~Process() {
    //std::cout << "Process " << ID  << " destroyed" << std::endl;
}

void Process::DecreaseTime(float time) { //Descuenta el tiempo
    (timeleft < time) ? timeleft = 0.f: timeleft -= time; 
}

void Process::print() {
    std::cout << "ID: " << ID << "\ttt: " << turntime << "\tAt: "<< arrival_time << "\ttleft: " << timeleft << "\tp: "<< priority << std::endl; 
}

bool Process::isDone(){ 
    if(timeleft == 0.f) {
        //std::cout << "Process " << ID << " finished" << std::endl;
        return true; 
    } else if (timeleft <= 0.f) { 
        timeleft = 0.f; 
        //std::cout << "Process " << ID << " finished" << std::endl;
        return true;
    } else 
        return false; 
}