/* 	Universidade de Brasilia
	Gabriel Levi Gomes da Silva
	Matricula - 16/0006490
	Trabalho Prático 1 - Programacao concorrente
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t cups[4];

typedef struct queue_item {
    int value;
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

void insert(queue * p, int item){
    queue_item * element = (queue_item *)malloc(sizeof(queue_item));
    element->value = item;
    element->next = NULL;
    if(p->first == NULL){
        p->first = element;
    }
    else{
        p->last->next = element;
    }
    p->last = element;
}

void * rmv(queue * p){
    queue_item * aux = p->first->next;
    free(p->first);
    p->first = aux;
}

void printQueue(queue * p){
    queue_item * atual = p->first;
    while(atual != NULL){
          printf("%d\n",atual->value);
          atual = atual->next;
    }
    printf("end of queue\n");
}

void * scheduler(pthread_t client_thread, int priority){
    

    pthread_exit(0);
}

void * thread1(void * arg){
    int id = *((int) * arg);

}

int main(){
    queue * priority_queue = createQueue();
    return 0;
}