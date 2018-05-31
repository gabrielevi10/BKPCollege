#include "thread.h"

void * thread(void * arg){
    int id = *((int*) arg);
    int priority;
    sleep(1);
    while(1){    
        priority = rand() % NUMBEROFTHREADS;
        pthread_mutex_lock(&queue_access);
        printf("Thread %d with priority %d wants execute\n", id, priority);
        insertInQueue(priority_queue, id, priority);
        pthread_mutex_unlock(&queue_access);
        sem_wait(&cups[id]);
        printf("Thread %d is allowed to execute\n", id);
        while(executing[id] == 1){
            printf("Thread %d executando\n", id);
            pthread_mutex_lock(&arrays_mutex);
            time_in_execution[id]++;
            pthread_mutex_unlock(&arrays_mutex);     
            sleep(1);
        }
        sleep(1);
    }
    pthread_exit(0);
}