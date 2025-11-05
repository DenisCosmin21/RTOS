//
// Created by Denis on 10/27/2025.
//

#include "task.h"

TCB_t newTask(const priority_t priority,const int value) {
    return (TCB_t) {priority, value};
}