#include "task.h"
#include "memory_pool.h"

TCB_t init_task(const priority_t priority,const int stack_size, const int execution_time, const int period) {
    void *stack_pointer = get_buffer(stack_size);
    TCB_t new_task;
    new_task.priority = priority;
    new_task.stack_size = stack_size;
    new_task.stack_pointer = stack_pointer;
    new_task.base_stack_pointer = stack_pointer;
    new_task.execution_time = execution_time;
    new_task.period = period;
    new_task.next_release_time = 0;
    new_task.remaining_time = execution_time;
    return new_task;
}

void destruct_task(TCB_t *task) {
    free_buffer(task->base_stack_pointer, task->stack_size);
}

TCB_t empty_task(void) {
    TCB_t task = {-1, 0, 0, 0, 0, 0, 0, 0};

    return task;
}

short is_empty_task(const TCB_t *task) {
    return
    task->priority == -1 &&
    task->stack_pointer == 0 &&
    task->base_stack_pointer == 0 &&
    task->execution_time == 0 &&
    task->period == 0 &&
    task->remaining_time == 0;
}

short task_is_ready(const TCB_t *task) {
    return task->next_release_time >= current_time;
}