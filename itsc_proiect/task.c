#include "task.h"

TCB_t newTask(const priority_t priority, const int value) {
    return (TCB_t) { priority, value };
}