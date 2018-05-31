#ifndef SCHEDULER_H
#define SCHEDULER_H


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h> 

#include "queue.h"

#define NUMBEROFTHREADS 8

extern sem_t cups[NUMBEROFTHREADS];
extern pthread_mutex_t queue_access;
extern pthread_mutex_t arrays_mutex;
extern pthread_mutex_t time_mutex;
extern int priority_in_execution[NUMBEROFTHREADS];
extern int execution[4];
extern int can_execute[8];
extern int last_executed[4];
extern struct queue *priority_queue;
extern int time_in_execution[NUMBEROFTHREADS];

void *scheduler();

#endif