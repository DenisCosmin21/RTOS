#pragma once
#ifndef TASK_H
#define TASK_H

typedef enum {
    VERY_HIGH,
    HIGH,
    MEDIUM,
    LOW
} priority_t;

typedef struct TCB {
    priority_t priority;     // Fixed priority
    int executionTime;       // C: worst-case execution time
    int period;              // T: period of task
    int remainingTime;       // Remaining execution time in current period
    int nextReleaseTime;     // Time of next release
    const char* name;        // Optional: task name for debugging
} TCB_t;

TCB_t newTask(const char* name, int executionTime, int period, priority_t priority);
TCB_t createEmptyTask(void);


#endif //TASK_H