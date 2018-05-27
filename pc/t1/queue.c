#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

typedef struct queue_item {
    int id;
    int priority; 
    struct queue_item * next;
} queue_item;

typedef struct queue {
    queue_item * first;
    queue_item * last;
} queue;

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

void * removeFromQueue(queue * p){
    queue_item * aux = p->first->next;
    free(p->first);
    p->first = aux;
}

void printQueue(queue * p){
    queue_item * atual = p->first;
    while(atual != NULL){
          printf("Na fila tem o %d\n",atual->id);
          atual = atual->next;
    }
    printf("end of queue\n");
}

int main(){
    queue *q = createQueue();
    
    insertInQueue(q, 1, 1);
    insertInQueue(q, 2, 1);
    insertInQueue(q, 3, 1);
    insertInQueue(q, 4, 1);
    insertInQueue(q, 5, 1);
    insertInQueue(q, 6, 1);
    printQueue(q);
    return 0;
}