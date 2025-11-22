#include "task.h"
#include <stdio.h>
#include <string.h>
#include "../Memory/memory_pool.h"
#include "../RmsScheduler/rms_scheduler.h"
#include "../RtosApi/rtos.h"

TCB_t *running_task = 0x00;

TCB_t *init_task(const priority_t priority,const int stack_size, const int execution_time, const int period, const char *name) {
    TCB_t *task = c_malloc(sizeof(TCB_t));
    void *stack_pointer = c_malloc(stack_size * sizeof(int));
    task->priority = priority;
    task->stack_size = stack_size;
    task->stack_pointer = stack_pointer;
    task->base_stack_pointer = stack_pointer;
    task->execution_time = execution_time;
    task->period = period;
    task->next_release_time = 0;
    task->remaining_time = execution_time;
    strcpy(task->name, name);
    return task;
}

void destruct_task(TCB_t *task) {
    c_free(task->base_stack_pointer);
    c_free(task);
}

TCB_t empty_task(void) {
    TCB_t task = {{0}, EMPTY, 0, 0, 0, 0, 0, 0, 0};

    return task;
}

short is_empty_task(const TCB_t *task) {
    return task->priority == EMPTY;
}

short task_is_ready(const TCB_t *task) {
    return task->next_release_time >= current_time;
}

TCB_t *context_switch(TCB_t *next_task) {
    TCB_t *waiting_task = running_task;
    running_task = next_task;
    if(waiting_task != 0x00 && waiting_task->remaining_time == 0) {
        waiting_task->next_release_time = current_time + (waiting_task->period - (current_time % waiting_task->period));
    }
    return waiting_task;
}

short should_switch() {
    if(running_task == 0x00)
        return 1;

    running_task->remaining_time--;

    if(should_preempt(running_task) || running_task->remaining_time == 0)
        return 1;

    return 0;
}

void print_task(const TCB_t *task) {
    printf("%s, ", task->name);
}