#include "task.h"
#include <stdio.h>
#include <string.h>
#include "../Memory/memory_pool.h"
#include "../RmsScheduler/rms_scheduler.h"
#include "../RtosApi/rtos.h"
#include "../Globals/globals.h"


TCB_t *init_task(const unsigned long priority,const int stack_size, const int execution_time, const int period, const char *name) {
    TCB_t *task = c_malloc(sizeof(TCB_t));
    void *stack_pointer = c_malloc(stack_size * sizeof(int));
    task->priority = priority;
    task->stack_size = stack_size;
    task->stack_pointer = stack_pointer;
    task->base_stack_pointer = stack_pointer;
    task->worst_case_execution_time = execution_time;
    task->period = period;
    task->next_release_time = 0;
    task->budget_time = execution_time;
    strcpy(task->name, name);
    return task;
}

void destruct_task(TCB_t *task) {
    c_free(task->base_stack_pointer);
    c_free(task);
}

short task_is_ready(const TCB_t *task) {
    return task->next_release_time >= current_time;
}

void reset_task(TCB_t *task) {
    task->next_release_time = current_time + (task->period - (current_time % task->period));
}

void context_switch() {
    running_task = next_task;
}

void should_switch() {
    if(running_task == 0x00) {
        if(exists_any_task())
            rtos_task_wait();
        return;
    }

    running_task->budget_time--;

    if(exists_higher_priority_task(running_task))
        rtos_task_yeld();

    if(running_task->budget_time <= 0)
        rtos_task_wait();
}

void print_task(const TCB_t *task) {
    printf("%s, ", task->name);
}