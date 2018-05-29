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
#include <time.h>

#define NUMBEROFTHREADS 8

sem_t cups[NUMBEROFTHREADS];
pthread_mutex_t queue_access = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t arrays_mutex = PTHREAD_MUTEX_INITIALIZER;
int priority_in_execution[NUMBEROFTHREADS];
int execution[4] = {-1, -1, -1, -1};
int executing[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int last_executed[4] = {-1, -1, -1, -1};
struct queue * priority_queue;

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

void * removeFromQueue(queue * q){
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

void print_array(int v[], int index){
    printf("Array = ");
    for(int i = 0; i < index; i++)
        printf("%d", v[i]);
    printf("\n");
}   

void * scheduler(){
    queue * stoped = createQueue();
    int i, flag;
    int aux;
    while(1){
        if(stoped->first != NULL){
            sem_post(&cups[stoped->first->id]);
            priority_in_execution[i] = stoped->first->id;
            printf("A thread %d voltou a executar\n", stoped->first->id);
            removeFromQueue(stoped);
            sleep(5);
            pthread_mutex_unlock(&arrays_mutex);
        }
        pthread_mutex_lock(&queue_access);
        if(priority_queue->first != NULL){
            // printf("O escalonador pegou o começo da fila\n");
            // printf("id = %d \n", priority_queue->first->id);
            flag = 0;
            pthread_mutex_lock(&arrays_mutex);
            for(i = 0; i < 4; i++){
                if(execution[i] == -1 && flag == 0 
                        && last_executed[i] != priority_queue->first->priority){
                    sem_post(&cups[priority_queue->first->id]);
                    execution[i] = priority_queue->first->id;
                    last_executed[i] != priority_queue->first->id;
                    executing[priority_queue->first->id] = 1;
                    priority_in_execution[i] = priority_queue->first->priority;
                    flag = 1;
                }
            }
            pthread_mutex_unlock(&arrays_mutex);
            if(flag = 0){
                pthread_mutex_lock(&arrays_mutex);
                for(i = 0; i < 4; i++){
                    if(priority_in_execution[i] < priority_queue->first->priority && flag == 0 
                            && last_executed[i] != priority_queue->first->priority){
                        insertInQueue(stoped, execution[i], priority_in_execution[i]);
                        printf("A thread %d foi interrompida para execução de outra com maior prioridade\n", execution[i]);
                        sem_post(&cups[priority_queue->first->id]);
                        executing[priority_queue->first->id] = 1;
                        priority_in_execution[i] = priority_queue->first->priority;
                        flag = 1;
                    }
                }
                pthread_mutex_unlock(&arrays_mutex);
            }
            removeFromQueue(priority_queue);
        }
        pthread_mutex_unlock(&queue_access);
        sleep(1);
    }
    pthread_exit(0);
}

void * thread(void * arg){
    int id = *((int*) arg);
    int i = 0;
    int priority;
    while(1){    
        srand(time(NULL));
        priority = rand() % NUMBEROFTHREADS;
        pthread_mutex_lock(&queue_access);
        printf("Thread %d wants execute\n", id);
        insertInQueue(priority_queue, id, priority);
        pthread_mutex_unlock(&queue_access);
        sem_wait(&cups[id]);
        printf("Thread %d is allowed to execute\n", id);
        pthread_mutex_lock(&arrays_mutex);
        while(executing[id] == 1){
            pthread_mutex_unlock(&arrays_mutex);
            pthread_mutex_lock(&arrays_mutex);
        }
        printf("Thread %d was interrupted by the scheduler\n", id);
        sem_post(&cups[0]);
        pthread_mutex_unlock(&arrays_mutex);
        sleep(1);
    }
    pthread_exit(0);
}

// void * firefox(){
//     int priority;
//     while(1){    
//         srand(time(NULL));
//         priority = rand() % NUMBEROFTHREADS;
//         pthread_mutex_lock(&queue_access);
//         printf("Text Editor wants execute\n");
//         insertInQueue(priority_queue, 1, priority);
//         pthread_mutex_unlock(&queue_access);
//         sem_wait(&cups[0]);
//         printf("Text Editor is allowed to execute\n");
//         pthread_mutex_lock(&arrays_mutex);
//         while(executing[0] == 1){
//             pthread_mutex_unlock(&arrays_mutex);
//             pthread_mutex_lock(&arrays_mutex);
//         }
//         printf("Text Editor was interrupted by the scheduler\n");
//         sleep(1);
//         pthread_mutex_unlock(&arrays_mutex);
//     }
// }

// void * avast(){
    
// }

// void * bash(){
    
// }

// void * dropbox(){
    
// }

// void * configuration(){
    
// }

// void * software_interruption(){
    
// }

// void * file_management(){
    
// }

int main(){
    priority_queue = createQueue();
    pthread_t threads[NUMBEROFTHREADS];
    pthread_t f_scheduler;
    int * id; 
    int i;
    
    for(i = 0; i < NUMBEROFTHREADS; i++){
        sem_init(&cups[i], 0, 0);
    }

    pthread_create(&f_scheduler, NULL, scheduler, NULL);

    for(i = 1; i < NUMBEROFTHREADS+1; i++){
        id = (int *)malloc(sizeof(int));
        *id = i;
        pthread_create(&threads[i], NULL, thread, (void *)id);
    }

    pthread_join(f_scheduler, NULL);
    for(i = 0; i < NUMBEROFTHREADS; i++){
        pthread_join(threads[i], NULL);
    }
    return 0;
}