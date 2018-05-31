/* 	Universidade de Brasilia
	Gabriel Levi Gomes da Silva
	Matricula - 16/0006490
	Trabalho Prático 1 - Programacao Concorrente
*/ 

#include "scheduler.h"

void * scheduler(){
    int i, flag;
    while(1){
        flag = 0;
        pthread_mutex_lock(&queue_access);
        if(priority_queue->first != NULL){
            /*Verifica se tem espaço para execução de uma thread*/
            for(i=0; i < 4; i++){
                pthread_mutex_lock(&arrays_mutex);
                if(execution[i] == -1 && flag == 0){
                    execution[i] = priority_queue->first->id;
                    executing[priority_queue->first->id] = 1;
                    priority_in_execution[priority_queue->first->id] = priority_queue->first->priority;
                    sem_post(&cups[priority_queue->first->id]);
                    sleep(1);
                    flag = 1;
                }
                pthread_mutex_unlock(&arrays_mutex);
            }
            /*Não encontrou espaço para executar, portanto vai verificar se tem 
            alguma thread de menor prioridad executando para que a mesma seja bloqueada */
            if(flag == 0){
                for(i = 0; i < 4; i++){
                    pthread_mutex_lock(&arrays_mutex);
                    if(priority_in_execution[execution[i]] <= priority_queue->first->priority && flag == 0){
                        executing[execution[i]] = 0;
                        printf("Thread %d was interrupted by the scheduler for execute %d \n", execution[i], priority_queue->first->id);
                        execution[i] = priority_queue->first->id;
                        executing[priority_queue->first->id] = 1;
                        priority_in_execution[priority_queue->first->id] = priority_queue->first->priority;
                        sem_post(&cups[priority_queue->first->id]);
                        sleep(1);
                        flag = 1;
                    }
                    pthread_mutex_unlock(&arrays_mutex);
                }
                /*Verifica se tem alguma thread com um tempo extendido de execução
                para dar a vez a outra thread*/
                for(i=0; i < 4; i++){
                    pthread_mutex_lock(&arrays_mutex);
                    if(time_in_execution[execution[i]] > 10 && flag == 0){
                        executing[execution[i]] = 0;
                        printf("Thread %d was interrupted by the scheduler for execute %d because of time in execution \n", execution[i], priority_queue->first->id);
                        execution[i] = priority_queue->first->id;
                        executing[priority_queue->first->id] = 1;
                        priority_in_execution[priority_queue->first->id] = priority_queue->first->priority;
                        sem_post(&cups[priority_queue->first->id]);
                        sleep(1);
                        flag = 1;
                    }
                    pthread_mutex_unlock(&arrays_mutex);
                }
            }
            removeFromQueue(priority_queue);
        }
        pthread_mutex_unlock(&queue_access);
        sleep(1);
    }
    pthread_exit(0);
}