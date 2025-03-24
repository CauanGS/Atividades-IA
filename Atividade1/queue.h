#include <stdio.h>
#include "vacuum.h"
#include <stdbool.h>

typedef struct Node{
    struct Node* next;
    place val; //Local que o nó armazena
}Node; //Struct para o nó
 

typedef struct{
    int size;
    Node *front;
    Node *rear;
}Queue; //struct para a fila

Queue newQueue();

int queue_empty(Queue* q);
int queue_insert(Queue* q, place p);
int queue_pop(Queue* q);
int queue_front(Queue* q, place* elem);
void delQueue(Queue* q);