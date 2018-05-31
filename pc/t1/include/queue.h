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