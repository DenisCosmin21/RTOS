//
// Created by Denis on 11/5/2025.
//

#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

void *get_buffer(const int buffer_size);

void free_buffer(const void *buffer, const int buffer_size);
#endif //MEMORY_POOL_H
