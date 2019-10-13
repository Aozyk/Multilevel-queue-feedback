#ifndef PROCESS_H
#define PROCES_H

class Process { //Clase proceso
    private:
        int ID;             // ID
        float turntime;     // Tiempo de proceso (rafagas)
        float timeleft;     // Tiempo restante (rafagas remanentes)
        /* bool visited = false; */
        
    public:
        Process(); //Proceso default
        Process(const Process& _rhs);
        Process(int id, float ttime, float arrival, int _priority) : ID(id), turntime(ttime), arrival_time(arrival), timeleft(ttime), priority(_priority)  {}
        ~Process(); //dtor

        float arrival_time; // Tiempo de llegada <- sólo será un timestamp
        int priority;

        float getTimeleft() {return timeleft;}
        float getID() {return ID;}

        /* bool getVisited() {return visited; }
        void markVisited() {visited = true; }
        void resetVisisted() {visited = false; }
         */void DecreaseTime(float time); //Descontar el tiempo
        bool isDone();
        void print();

};

#endif