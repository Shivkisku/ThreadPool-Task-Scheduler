#include "task_scheduler.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void sample_task(void* arg) {
    int* num = (int*)arg;
    printf("Task processing number: %d (Thread: %lu)\n", 
           *num, pthread_self());
    sleep(1); // Simulate work
    free(num);
}

int main() {
    TaskScheduler scheduler;
    init_scheduler(&scheduler);
    
    // Add tasks to scheduler
    for(int i = 0; i < 10; i++) {
        int* num = malloc(sizeof(int));
        *num = i;
        add_task(&scheduler, sample_task, num);
    }
    
    // Allow time for task processing
    sleep(5);
    
    cleanup_scheduler(&scheduler);
    printf("All tasks completed successfully\n");
    return 0;
}