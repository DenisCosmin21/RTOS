//
// Created by Denis on 12/1/2025.
//

#include "mutex.h"

#include "../Globals/globals.h"
#include "../RmsScheduler/rms_scheduler.h"
#include "../RtosApi/rtos.h"

static int enqueue_condition(const TCB_t *existent_task, const TCB_t *task_to_add) {
    return existent_task->priority < task_to_add->priority;
}

static int dequeue_condition(const TCB_t *task_to_delete) {
    return 1;
}

void mutex_init(mutex_t *mutex) {
    h_init(&mutex->tasks);
    mutex->status = 0;
}

static void locked_mutex(mutex_t *mutex) {
    if(running_task == mutex->current_task)
        return;

    h_enqueue(&mutex->tasks, running_task, enqueue_condition);

    running_task->blocked_by = mutex;

    if(mutex->current_task->priority < running_task->priority) {
        inheritate_priority(mutex->current_task, running_task);
    }

    next_task = scheduler_get_task();
    context_switch();
}

static void unlocked_mutex(mutex_t *mutex) {
    mutex->status = 1;
    mutex->current_task = running_task;
}

void mutex_lock(mutex_t *mutex) {
    if(mutex->status == 0)
        unlocked_mutex(mutex);
    else
        locked_mutex(mutex);
}

void mutex_unlock(mutex_t *mutex) {
    if(mutex->status == 1 && mutex->current_task == running_task) {
        if(!h_is_empty(&mutex->tasks)) {
            mutex->current_task = h_dequeue(&mutex->tasks, dequeue_condition, enqueue_condition);
            mutex->current_task->temporary_priority = mutex->current_task->priority;
            scheduler_add_task(mutex->current_task);
        }
        else
            mutex->status = 0;

        running_task->temporary_priority = MAX_PRIORITY_COUNT;
        running_task->blocked_by = 0x00;
    }
}