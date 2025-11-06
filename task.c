//
// Created by Denis on 10/27/2025.
//

#include "task.h"
#include "memory_pool.h"

TCB_t init_task(const priority_t priority,const int stack_size) {
    void *stack_pointer = get_buffer(stack_size);
    TCB_t new_task;
    new_task.priority = priority;
    new_task.stack_size = stack_size;
    new_task.stack_pointer = stack_pointer;
    new_task.base_stack_pointer = stack_pointer;
    return new_task;
}

void destruct_task(TCB_t *task) {
    free_buffer(task->base_stack_pointer, task->stack_size);
}