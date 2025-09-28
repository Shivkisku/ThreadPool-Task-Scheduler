#include "task_scheduler.h"
#include <stdlib.h>
#include <stdio.h>

void init_scheduler(TaskScheduler* ts) {
    ts->task_count = 0;
    ts->shutdown = false;
    pthread_mutex_init(&ts->queue_mutex, NULL);
    pthread_cond_init(&ts->queue_cond, NULL);
    
    for(int i = 0; i < MAX_THREADS; i++) {
        pthread_create(&ts->threads[i], NULL, (void*)execute_tasks, ts);
    }
}

void add_task(TaskScheduler* ts, void (*function)(void*), void* arg) {
    pthread_mutex_lock(&ts->queue_mutex);
    if(ts->task_count < MAX_TASKS) {
        Task* task = &ts->tasks[ts->task_count++];
        task->function = function;
        task->arg = arg;
        task->completed = false;
        pthread_cond_signal(&ts->queue_cond);
    }
    pthread_mutex_unlock(&ts->queue_mutex);
}

void execute_tasks(TaskScheduler* ts) {
    while(1) {
        pthread_mutex_lock(&ts->queue_mutex);
        while(ts->task_count == 0 && !ts->shutdown) {
            pthread_cond_wait(&ts->queue_cond, &ts->queue_mutex);
        }
        
        if(ts->shutdown) {
            pthread_mutex_unlock(&ts->queue_mutex);
            return;
        }
        
        Task* task = &ts->tasks[--ts->task_count];
        pthread_mutex_unlock(&ts->queue_mutex);
        
        task->function(task->arg);
        task->completed = true;
    }
}

void cleanup_scheduler(TaskScheduler* ts) {
    pthread_mutex_lock(&ts->queue_mutex);
    ts->shutdown = true;
    pthread_cond_broadcast(&ts->queue_cond);
    pthread_mutex_unlock(&ts->queue_mutex);
    
    for(int i = 0; i < MAX_THREADS; i++) {
        pthread_join(ts->threads[i], NULL);
    }
    
    pthread_mutex_destroy(&ts->queue_mutex);
    pthread_cond_destroy(&ts->queue_cond);
}