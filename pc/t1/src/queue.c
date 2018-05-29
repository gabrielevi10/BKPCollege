#include "queue.h"

queue * createQueue(){
    queue *q = (queue *)malloc(sizeof(queue));
    q->first = NULL;
    q->last = NULL;
    return q;
}

void insertInQueue(queue * p, int id, int priority){
    queue_item * element = (queue_item *)malloc(sizeof(queue_item));
    element->id = id;
    element->priority = priority;
    element->next = NULL;
    if(p->first == NULL){
        p->first = element;
    }
    else{
        p->last->next = element;
    }
    p->last = element;
}

void removeFromQueue(queue * q){
    queue_item *removido = q->first;
    q->first = q->first->next;
    free(removido);
    if(q->first == NULL){
       q->last = NULL;
    } 
}

void printQueue(queue * p){
    queue_item * atual = p->first;
    while(atual != NULL){
          printf("Na fila tem o %d\n",atual->id);
          atual = atual->next;
    }
    printf("end of queue\n");
}