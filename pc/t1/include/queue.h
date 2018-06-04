/* 	Universidade de Brasilia
	Gabriel Levi Gomes da Silva
	Matricula - 16/0006490
	Trabalho Prático 1 - Programacao Concorrente
*/

#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

typedef struct queue_item {
    int id;
    int priority; 
    struct queue_item *next;
} queue_item;

typedef struct queue {
    queue_item *first;
    queue_item *last;
} queue;

queue *createQueue();

void insertInQueue(queue *p, int id, int priority);

void removeFromQueue(queue * q);

void printQueue(queue *p);

#endif