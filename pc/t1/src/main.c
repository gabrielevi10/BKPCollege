/* 	Universidade de Brasilia
	Gabriel Levi Gomes da Silva
	Matricula - 16/0006490
	Trabalho Prático 1 - Programacao Concorrente
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#include "queue.h"
#include "scheduler.h"
#include "thread.h"

#define NUMBEROFTHREADS 8

sem_t cups[NUMBEROFTHREADS];
pthread_mutex_t queue_access = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t arrays_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t time_mutex = PTHREAD_MUTEX_INITIALIZER;
int priority_in_execution[NUMBEROFTHREADS];
int execution[4] = {-1, -1, -1, -1};
int can_execute[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int last_executed[4] = {-1, -1, -1, -1};
int time_in_execution[NUMBEROFTHREADS];
struct queue *priority_queue;

int main(){
    priority_queue = createQueue();
    pthread_t threads[NUMBEROFTHREADS];
    pthread_t f_scheduler;
    int * id; 
    int i;
    
    for(i = 0; i < NUMBEROFTHREADS; i++){
        priority_in_execution[i] = -1;
        time_in_execution[i] = 0;
        sem_init(&cups[i], 0, 0);
    }

    pthread_create(&f_scheduler, NULL, scheduler, NULL);
    for(i = 0; i < NUMBEROFTHREADS; i++){
        id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&threads[i], NULL, thread, (void *)id);
    }

    pthread_join(f_scheduler, NULL);
    pthread_join(threads[0], NULL);
    
    return 0;
}