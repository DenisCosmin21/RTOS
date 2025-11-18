#include "task.h"
#include <stddef.h>


TCB_t newTask(const char* name, int executionTime, int period, priority_t priority) {
    TCB_t t;
    t.name = name;
    t.executionTime = executionTime;
    t.period = period;
    t.remainingTime = executionTime;
    t.nextReleaseTime = 0;
    t.priority = priority;

    return t;
}
TCB_t createEmptyTask(void) {
    TCB_t empty;
    empty.priority = LOW;
    empty.executionTime = -1;
    empty.period = -1;
    empty.remainingTime = -1;
    empty.nextReleaseTime = -1;
    empty.name = NULL;
    return empty;
}