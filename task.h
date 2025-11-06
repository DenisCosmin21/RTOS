//
// Created by Denis on 10/27/2025.
//

#ifndef TASK_H
#define TASK_H

typedef enum {
    VERY_HIGH,
    HIGH,
    MEDIUM,
    LOW
}priority_t;

typedef struct TCB {
    priority_t priority;
    void *stack_pointer;
    void *base_stack_pointer;
    int stack_size;
} TCB_t;

TCB_t init_task(priority_t priority,const int stack_size);

void destruct_task(TCB_t *task);
#endif //TASK_H
