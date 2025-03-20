#include "queue.h"


Queue newQueue(){
    Queue q;
    q.front = NULL;
    q.rear = NULL;
    q.size = 0; 
    return q;
}

int queue_empty(Queue* q){
    return q->size == 0;
}

int queue_insert(Queue* q,place p){
    
    Node* node = malloc(sizeof(struct Node));
    if(node == NULL){
        printf("Não foi possível alocar memória para o nó\n");
        return 0;
    }
    node->val = p;
    node->next = NULL;
    if(queue_empty(q)){
        q->front = node;
        q->rear = node;
    }
    else{
        q->rear->next = node;
        q->rear = node;
    }
    q->size++;
    return 1;
}

int queue_pop(Queue* q){
    if(queue_empty(q)){
        return 0;
    }
    q->size--;
    Node* curr = q->front;
    q->front = q->front->next;
    free(curr);
    return 1;
}

int queue_top(Queue* q, place* elem){
    if(queue_empty(q)){
        return 0;
    }
    *elem = q->front->val;
    return 1;
}

void delQueue(Queue* q){
    if(q == NULL){
        return;
    }
    Node* curr = q->front;
    while(curr != NULL){
        q->front = q->front->next;
        free(curr);
        curr = q->front;
    }
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}