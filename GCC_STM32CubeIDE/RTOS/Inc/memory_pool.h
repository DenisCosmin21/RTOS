//
// Created by Denis on 11/5/2025.
//

#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#define HEAP_SIZE 4096//Should be enough memory
#include "rtos_config.h"

void *c_malloc(const short);

void c_free(void *);
#endif //MEMORY_POOL_H
