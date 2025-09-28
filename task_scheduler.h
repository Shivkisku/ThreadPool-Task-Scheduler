#ifndef TASK_SCHEDULER_H
#define TASK_SCHEDULER_H

#include <pthread.h>
#include <stdbool.h>

#define MAX_TASKS 10
#define MAX_THREADS 4

typedef struct {
    void (*function)(void*);
    void* arg;
    bool completed;
} Task;

typedef struct {
    Task tasks[MAX_TASKS];
    int task_count;
    pthread_t threads[MAX_THREADS];
    pthread_mutex_t queue_mutex;
    pthread_cond_t queue_cond;
    bool shutdown;
} TaskScheduler;

void init_scheduler(TaskScheduler* ts);
void add_task(TaskScheduler* ts, void (*function)(void*), void* arg);
void execute_tasks(TaskScheduler* ts);
void cleanup_scheduler(TaskScheduler* ts);

#endif