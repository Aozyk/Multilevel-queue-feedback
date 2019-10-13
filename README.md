# Multilevel queue feedback
Trabajo para la asignatura de Sistemas Operativos 2019-2 de la Universidad Tecnológica Metropolitana (UTEM) que implementa una cola multinivel retroalimentada en C++. Las subcolas en orden son:
- Round robin (q = 2)
- Round robin (q = 4)
- Priority Apropiativo
- FCFS
#### Autores
- Daniel Aguilera T.
- Nicolás Andrews S.
- Iván Pérez A.

### Aclaraciones
El búfer corresponde a una EDD que permite marcar los procesos que ya han sido cumplido su tiempo en la cola y están esperando para ingresar a la cola siguiente.

El nivel corresponde a la cola activa, siendo 0 la primera cola y 3 la última.

Al presionar una tecla, se genera una interrupción por lo que el sistema no está contando el tiempo de los procesos en las colas respectivas.

### Instrucciones de uso
Al presionar una tecla trae el menu para agregar un nuevo proceso pidiendo al usuario que ingrese los datos requeridos por teclado
- ID (entero)
- Tiempo de rafaga (decimal o entero)
- Prioridad (entero) 

### Compilación
Utilizar C++14 (GNU)
Comando:
```
g++ -std=gnu++14 -o multiqueue.exe main.cpp adminqueue.cpp process.cpp
```
