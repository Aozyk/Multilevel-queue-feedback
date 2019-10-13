#include "adminqueue.h"
#include <iostream>
#include <windows.h>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;

AdminQ::AdminQ() {
    adminTime = 0.f;
    subTime = 0.f;
}

AdminQ::~AdminQ() {
}

void AdminQ::resetTimers() {
    adminTime = 0.f;
    subTime = 0.f;
}

void AdminQ::changeLevel() { //Para transiciones
    int anotherLevel = currentLevel;
    for(int i = 0; i < currentLevel; i++) { // check 0-current
        if(!rQ[i].empty()) {
            anotherLevel = i;
        }
    }

    int nextLevel = currentLevel;
    for(int i = 3; i > currentLevel; i--) {
        if(!rQ[i].empty()) {
            nextLevel = i;
        }
    }
/*     deberiamos chequear si la cola siguiente tiene procesos, en ese caso, chequeamos clvl +1, si los tiene,
        pasamos esa cola al vector, luego la cola actual al vector, y luego movemos el vector a la cola+1
    para anotherlvl = currentlevel -> submimos un lvl pasando la cola restante al vector y devolviendola a la cola +1lvl
    para anotherlvl < currentlevle -> vamos al nivel encotnrado pero no nos llevamos los procesos
    para nextlvl > currentlevel -> avanzamos 1 lvl de manera normal tomando la precaución de arriba. */

    if(anotherLevel == currentLevel && nextLevel == currentLevel) {
        flushToVector(currentLevel);
        if(currentLevel != 3) {
            currentLevel += 1;
        }
        flushToQueue(currentLevel);
    }else if (anotherLevel < currentLevel) {
        flushToQueue(currentLevel); //Le devolvemos los procesos al actual si es que sacamos alguno
        currentLevel = anotherLevel;
    } else if (nextLevel > currentLevel) {
        if(currentLevel != 3) {
            if(rQ[currentLevel+1].empty()) {
                flushToVector(currentLevel);
                currentLevel += 1;
            }
            else if(!rQ[nextLevel].empty()) {
                // huh
                flushToVector(currentLevel); // Movemos los procesos restantes de la cola al vector
                flushToQueue(currentLevel); // Movemos de vuelta a la cola para mantener el orden del siguiente nivel

                flushToVector(nextLevel); // Mover los procesos de la cola objetivo 
                flushToVector(currentLevel); // Unir con el vector temporal
                currentLevel = nextLevel;
            }
            flushToQueue(currentLevel);
        }
    }
    resetTimers();
    
}

void AdminQ::resetSubTimer() {
    subTime = 0.f;
}

void AdminQ::flushToVector(int Lvl) {
    if(rQ[Lvl].empty()) {
        return;
    }
    while(!rQ[Lvl].empty()) { //Mientras la cola no esté vacía
        tempQ.push_back(rQ[Lvl].front());
        rQ[Lvl].pop();
    }
}

void AdminQ::flushToQueue(int Lvl) {
    if(tempQ.empty()) {
        return;
    }
    if(!rQ[Lvl].empty()) { // Si la cola a la que queremos mover tiene procesos, los movemos al vector primero, y luego operamos.
        flushToVector(Lvl);
    }
    if(Lvl == 2) { //Orden ascendente (priority max = 0)
        sort(tempQ.begin(), tempQ.end(), [](const Process& lhs, const Process& rhs) {
            return lhs.priority < rhs.priority;
        });
    }else if(Lvl == 3) { //Orden ascendente (FCFS)
        sort(tempQ.begin(), tempQ.end(), [](const Process& lhs, const Process& rhs) {
            return lhs.arrival_time < rhs.arrival_time;
        });
    }
    for (auto p : tempQ) {
        rQ[Lvl].push(p);
    }
    tempQ.clear(); //Limpiar el vector
}

void AdminQ::handleInput(float inArrival) {
    if(num_process == 10) {
        cout << "No se puede crear mas de 10 procesos, intente denuevo cuando haya finalizado un proceso" << endl;
        system("pause");
        return;
    }
    string inID, inTurntime, inPriority;
    cout << "Ingrese ID de proceso: ";
    cin >> inID;
    cout << "Ingrese tiempo de rafaga del proceso: ";
    cin >> inTurntime;
    cout << "Ingrese prioridad del proceso: ";
    cin >> inPriority;
    
    Process PP(stoi(inID), stof(inTurntime), inArrival, stoi(inPriority));
    // Siempre entra a la primera cola
    rQ[0].push(PP);
    if(currentLevel != 0) {
        resetTimers();
        currentLevel = 0;
        
    }
    num_process += 1;

    return;
}

bool AdminQ::checkAdminTime() {
    if(adminTime >= adminTimeConstraint) {
        adminTime = adminTimeConstraint;
        return true;
    }else {
        return false;
    }
}

bool AdminQ::checkSubTime() {
    if(currentLevel == 0) {
        if(subTime >= sub1TimeConstraint) {
            subTime = sub1TimeConstraint;
            return true;
        }else{
            return false;
        }
    }else if(currentLevel == 1) {
        if(subTime >= sub2TimeConstraint) {
            subTime = sub2TimeConstraint;
            return true;
        }else{
            return false;
        }
    }
    else if(currentLevel > 1) {
        if(subTime >= adminTimeConstraint) {
            subTime = adminTimeConstraint;
            return true;
        }else {
            return false;
        }
    }
}

void AdminQ::updateProcess(float time) {
    bool moveToNextQ = false;
    if(num_process == 0) { //Si no hay procesos no podemos updatearlos.
        return;
    }
    if(rQ[currentLevel].empty() && num_process == 0) { //Si no hay procesos en el sistema)
        return;
    }else if (rQ[currentLevel].empty() && num_process !=0){ // Si la cola está vacía pero hay procesos en el sistema
        changeLevel();
    }
    // Updatear timers
    adminTime += time;
    subTime += time;
    if(checkAdminTime()) {
        moveToNextQ = true;
    }

    // Updatear proceso
    rQ[currentLevel].front().DecreaseTime(time);
    if(rQ[currentLevel].front().isDone()) {
        rQ[currentLevel].pop();
        num_process -= 1;
        resetSubTimer();
    } else if (checkSubTime()) {
        tempQ.push_back(rQ[currentLevel].front());
        rQ[currentLevel].pop();
        if(rQ[currentLevel].empty()) {
            changeLevel();
        }
        resetSubTimer();
    }

    if(moveToNextQ) {
        changeLevel();
    }
}

string AdminQ::getProcessList(int Lvl) {
    queue<Process> qq(rQ[Lvl]);

    // caso en que la cola esté vacía
    string ret_val = "";
    std::ostringstream oss;
    if(qq.empty()) {
        return "Queue vacio";
    }
    while(!qq.empty()) {
        oss << qq.front().getID();
        ret_val += oss.str();
        if(qq.size() != 1) {
            ret_val += " -> ";
        }
        qq.pop();
        oss.str("");
    }

    return ret_val;
}

string AdminQ::debugTempVector () {
    ostringstream oss;
    string ret_val = "";
    if(tempQ.empty()) {
        return "No hay procesos";
    }
    for(int i = 0; i < tempQ.size(); i++) {
        oss << tempQ[i].getID();
        ret_val += oss.str();
        if(i != tempQ.size() -1) {
            ret_val += " - ";
        }
        oss.str("");
    }
    return ret_val;
}

void AdminQ::debug() {
    string q1 = getProcessList(0);
    string q2 = getProcessList(1);
    string q3 = getProcessList(2);
    string q4 = getProcessList(3);
    float ptime = 0.f;
    if(!rQ[currentLevel].empty()) {
       ptime = rQ[currentLevel].front().getTimeleft();
    }
    string currentProcess = "NULL";
    ostringstream oss;
    if(!rQ[currentLevel].empty()) {
        oss << rQ[currentLevel].front().getID();
        currentProcess = oss.str();
    }
    string vectorinfo = debugTempVector();
    cout << "Tiempo admin: " << adminTime << "\tTiempo Quanto: " << subTime
        << "\tCtd. Procesos: " << num_process << "\n\nNivel actual: " << currentLevel+1 << endl
        << "Queue 1 (R-R q:2)\t: " << q1 << endl 
        << "Queue 2 (R-R q:4)\t: " << q2 << endl 
        << "Queue 3 (PRIORITY)\t: " << q3 << endl 
        << "Queue 4 (FCFS)\t\t: " << q4 << endl 
        << "Proceso Actual: [" << currentProcess << "]\t" <<"Tiempo restante: " << ptime << endl
        << "-----------------------------\n"
        << "Buffer: " << vectorinfo << endl;
    oss.str("");
}
