/* 	Universidade de Brasilia
	Gabriel Levi Gomes da Silva
	Matricula - 16/0006490
	Trabalho Prático 1 - Programacao Concorrente
*/

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