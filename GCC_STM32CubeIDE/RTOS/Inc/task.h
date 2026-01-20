//
// Created by Denis on 10/27/2025.
//

#ifndef TASK_H
#define TASK_H
#define MAX_NAME_SIZE 20


typedef struct mutex mutex_t;

typedef struct TCB {
    void *stack_pointer;
    void *base_stack_pointer;
    char name[MAX_NAME_SIZE];
    unsigned long priority;
    unsigned long temporary_priority;
    mutex_t *blocked_by;
    int stack_size;
    int worst_case_execution_time;
    int period;
    int budget_time;
    int next_release_time;
    int went_to_sleep_time;
} TCB_t;

TCB_t *init_task(unsigned long priority,const int stack_size, const int execution_time, const int period, const char *name);

void destruct_task(TCB_t *);

short task_is_ready(const TCB_t *);

void context_switch();

void should_switch();

void reset_task(TCB_t *);

void inheritate_priority(TCB_t *task_that_inheritates,  TCB_t *task_inheritated);

void print_task(const TCB_t *);
#endif //TASK_H
