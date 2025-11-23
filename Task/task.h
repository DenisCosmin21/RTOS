//
// Created by Denis on 10/27/2025.
//

#ifndef TASK_H
#define TASK_H

typedef struct TCB {
    char name[20];
    unsigned long priority;
    void *stack_pointer;
    void *base_stack_pointer;
    int stack_size;
    int worst_case_execution_time;
    int period;
    int budget_time;
    int next_release_time;
} TCB_t;

TCB_t *init_task(unsigned long priority,const int stack_size, const int execution_time, const int period, const char *name);

void destruct_task(TCB_t *);

short task_is_ready(const TCB_t *);

TCB_t *context_switch();

short should_switch();

void print_task(const TCB_t *);
#endif //TASK_H
