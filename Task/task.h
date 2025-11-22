//
// Created by Denis on 10/27/2025.
//

#ifndef TASK_H
#define TASK_H

typedef enum {
    VERY_HIGH = 0,
    HIGH,
    MEDIUM,
    LOW,
    EMPTY,
    WAITING
}priority_t;

typedef struct TCB {
    char name[20];
    priority_t priority;
    void *stack_pointer;
    void *base_stack_pointer;
    int stack_size;
    int execution_time;
    int period;
    int remaining_time;
    int next_release_time;
} TCB_t;

extern TCB_t running_task;

TCB_t init_task(priority_t priority,const int stack_size, const int execution_time, const int period, const char *name);

void destruct_task(const TCB_t *task);

TCB_t empty_task(void);

short is_empty_task(const TCB_t *task);

short task_is_ready(const TCB_t *task);

TCB_t context_switch(TCB_t next);

short should_switch();

void print_task(const TCB_t *task);
#endif //TASK_H
