//
// Created by Denis on 11/5/2025.
//
#include "memory_pool.h"
#include <stddef.h>
#include <stdio.h>

#define LINE_SIZE 128
#define LINES 10

#define ADD_IF_NOT_DIVISIBLE(number, divizor) ((number) % (divizor) != 0 ? (number) / (divizor) + 1 : (number) / (divizor))

typedef struct{
    char pool[LINES][LINE_SIZE];
    char allocated_pools[ADD_IF_NOT_DIVISIBLE(LINES, sizeof(char) * 8)];
}memory_pool_t;

memory_pool_t memory_pool;

static int is_bit_set(size_t position) {
    size_t array_position = position / sizeof(char);
    size_t bit_position = position % sizeof(char);

    return memory_pool.allocated_pools[array_position] & (1 << bit_position);
}

static void set_bit(size_t position) {
    size_t array_position = position / (sizeof(char) * 8);
    size_t bit_position = position % (sizeof(char) * 8);

    memory_pool.allocated_pools[array_position] |= (1 << bit_position);
}

static void reset_bit(size_t position) {
    size_t array_position = position / sizeof(char);
    size_t bit_position = position % sizeof(char);

    memory_pool.allocated_pools[array_position] &= ~(1 << bit_position);
}

static int get_lines_count_from_buffer_size(const int buffer_size) {
    return ADD_IF_NOT_DIVISIBLE(buffer_size, LINE_SIZE);
}

void *get_buffer(const int buffer_size) {
    int lines_count = get_lines_count_from_buffer_size(buffer_size);

    int found = -1;
    int lines_found = 0;

    for(int i = 0; i < LINES; i++) {
        //Looking for lines_count adiacent spaces
        if(!is_bit_set(i) && found == -1) {
            found = i;
            lines_found++;
            set_bit(found);
            if(lines_found == lines_count)
                return memory_pool.pool[found];
        }
        else if(is_bit_set(i) && found != -1) {
            found = -1;

            while(lines_found) {
                reset_bit(i - lines_found);
                lines_found--;
            }
        }
        else if(!is_bit_set(i) && lines_found == lines_count - 1 && found != -1) {
            set_bit(i);
            return memory_pool.pool[found];
        }
        else {
            set_bit(i);
            lines_found++;
        }
    }

    return NULL;
}

void free_buffer(const void *buffer, const int buffer_size) {
    int lines_count = get_lines_count_from_buffer_size(buffer_size);

    ptrdiff_t position = (char *)buffer - memory_pool.pool[0];

    position = position % LINE_SIZE;

    for(ptrdiff_t i = position; i < lines_count; i++)
        reset_bit(i);
}
