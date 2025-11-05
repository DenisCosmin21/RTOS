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
    int value;
} TCB_t;

TCB_t newTask(priority_t priority, int value);
#endif //TASK_H
