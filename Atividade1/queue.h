#include <stdio.h>
#include "vacuum.h"
#include <stdbool.h>

/*
Nomes dos integrantes:
    Alexandre Jun Hayasaka 
    Cauan Gabriel de Souza
    Enzo Picelli Stevanato
    Felipe Pereira de Souza
    Gabriel Sanches Pinto
    Hannah Caroline Cavalcanti Santos Andrade
*/

typedef struct Node{
    struct Node* next;
    place* val; //Local que o nó armazena
}Node; //Struct para o nó
 

typedef struct{
    int size;
    Node *front;
    Node *rear;
}Queue; //struct para a fila

Queue newQueue();

int queue_empty(Queue* q);
int queue_insert(Queue* q, place* p);
place* queue_pop(Queue* q);
void delQueue(Queue* q);