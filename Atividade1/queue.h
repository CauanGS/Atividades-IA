#include <stdio.h>
#include "vacuum.h"

typedef struct Node{
    struct Node* next;
    place val;
    int time;
}Node;


typedef struct{
    int size;
    Node *front;
    Node *rear;
}Queue;

Queue newQueue();

int queue_empty(Queue* q);
int queue_insert(Queue* q, place p);
int queue_pop(Queue* q);
int queue_top(Queue* q, place* elem);
void delQueue(Queue* q);