#ifndef THREAD_H
#define THREAD_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h> 

#include "queue.h"
#include "scheduler.h"

void * thread(void * arg);

#endif