
#include <stdbool.h>
#include <stdlib.h>

/*
Código feito por:
    Alexandre Jun Hayasaka 
    Cauan Gabriel de Souza
    Enzo Picelli Stevanato
    Felipe Pereira de Souza
    Gabriel Sanches Pinto
    Hannah Caroline Cavalcanti Santos Andrade
*/

#define MAX_BATTERY 45
#define TRUE 1
#define FALSE 0
#define MOREINFO FALSE //se 1, printa informações extras no terminal

#ifndef VACUUM_H
#define VACUUM_H

//Propriedades de cada lugar do ambiente
typedef struct{
    int row, col; //indice do lugar em um grid
    bool dirt; //estado sujo
} place;

//O ambiente é uma matriz de lugares (grid)
typedef struct{
    int h, w;
    place** grid;
} enviroment;

enviroment newEnviroment(int h, int w);
void delEnviroment(enviroment* E);
void initEnviromentDirt(enviroment E, int numDirt);
bool isNeighbor(place p, place q);
place* getPlace(enviroment E, int i, int j);

typedef struct{
    place* whereCleaner;
    place* whereCharger;
    int battery;
    int numActions;
} cleaner;

cleaner newCleaner(enviroment);
bool move(cleaner* C, enviroment E, place* p);
bool goTarget(cleaner* C, enviroment E, place* target);
bool charge(cleaner* C, enviroment E);
void clean(cleaner* C);
int searchDirt(cleaner* C, enviroment E, place* pl);

void printSimulation(cleaner C, enviroment E);

#endif