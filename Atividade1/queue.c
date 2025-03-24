#include "queue.h"

/*
Nomes dos integrantes:
    Alexandre Jun Hayasaka 
    Cauan Gabriel de Souza
    Enzo Picelli Stevanato
    Felipe Pereira de Souza
    Gabriel Sanches Pinto
    Hannah Caroline Cavalcanti Santos Andrade
*/

Queue newQueue()
/*
    Cria nova fila
*/
{
    Queue q;
    q.front = NULL;
    q.rear = NULL;
    q.size = 0;
    return q;
}

int queue_empty(Queue *q)
/*
    Verifica se a fila está vazia
*/
{
    return q->size == 0;
}

int queue_insert(Queue *q, place p)
/*
    Insere novo nó na fila
*/
{

    Node *node = malloc(sizeof(struct Node));
    if (node == NULL)
    {
        printf("Não foi possível alocar memória para o nó\n");
        return 0;
    }
    node->val = p;
    node->next = NULL;
    if (queue_empty(q))
    {
        q->front = node;
        q->rear = node;
    }
    else
    {
        q->rear->next = node;
        q->rear = node;
    }
    q->size++;
    return 1;
}

int queue_pop(Queue *q)
/*
    Remove o nó da frente
*/
{
    if (queue_empty(q))
    {
        return 0;
    }
    q->size--;
    Node *curr = q->front;
    q->front = q->front->next;
    free(curr);
    return 1;
}

int queue_front(Queue *q, place *elem)
/*
    Retorna o valor do nó da frente
*/
{
    if (queue_empty(q))
    {
        return 0;
    }
    *elem = q->front->val;
    return 1;
}

void delQueue(Queue *q)
/*
    Deleta a fila
*/
{
    if (q == NULL)
    {
        return;
    }
    Node *curr = q->front;
    while (curr != NULL)
    {
        q->front = q->front->next;
        free(curr);
        curr = q->front;
    }
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}