//
// Created by Denis on 11/5/2025.
//
#include "memory_pool.h"
#include <stdint.h>
#include <stdio.h>

#define HEAP_SIZE 4096//Should be enough memory

#define GET_BLOCK_SIZE(memory_block) \
    ((((uint16_t)memory_block[0] << 8) | memory_block[1]) >> 1)

#define IS_BLOCK_OCUPPIED(memory_block) \
    memory_block[1] & 0x01

#define OCUPY_BLOCK(memory_block, new_size) \
    memory_block[0] = (new_size << 1) >> 8; \
    memory_block[1] = (new_size << 1) | 0x01; \

#define INITIALIZE_BLOCK(memory_block, size) \
    memory_block[0] = (size << 1) >> 8; \
    memory_block[1] = size << 1

#define NEXT_BLOCK(memory_block) \
    &memory_block[GET_BLOCK_SIZE(memory_block)]

#define MERGE_BLOCKS(memory_block1, memory_block2) \
    INITIALIZE_BLOCK(memory_block1, GET_BLOCK_SIZE(memory_block1) + GET_BLOCK_SIZE(memory_block2))

typedef uint8_t memory_pool_t;

static memory_pool_t memory_pool[HEAP_SIZE] = {[0] = 0x20, [1] = 0x00};

void *c_malloc(short size) {
    uint8_t *last_element = &memory_pool[HEAP_SIZE];
    uint8_t *current_element = memory_pool;
    uint16_t block_size = 0;

    while(1) {
        block_size = GET_BLOCK_SIZE(current_element);

        if (IS_BLOCK_OCUPPIED(current_element) || (block_size - sizeof(uint16_t))< size) {
            if(current_element + block_size >= last_element)
                return 0x00;
            current_element = NEXT_BLOCK(current_element);
            continue;
        }

        if(block_size - sizeof(uint16_t) == size) {
            OCUPY_BLOCK(current_element, size + sizeof(uint16_t));
            return current_element;
        }

        OCUPY_BLOCK(current_element, size + sizeof(uint16_t));
        uint8_t *block_to_initialize = NEXT_BLOCK(current_element);
        INITIALIZE_BLOCK(block_to_initialize, block_size - (size + sizeof(uint16_t)));
        return current_element + sizeof(uint16_t);
    }

    return 0x00;
}

static uint8_t *find_ant_block(uint8_t *current_element) {
    uint8_t *block = memory_pool;
    uint8_t *ant = block;

    while(block < current_element) {
        ant = block;
        block = NEXT_BLOCK(block);
    }

    return ant;
}

void c_free(void *ptr) {
    if(!(ptr - sizeof(uint16_t) >= memory_pool && ptr < memory_pool + HEAP_SIZE))
        return;

    uint8_t *element_to_free = ptr;
    element_to_free -= sizeof(uint16_t);

    uint8_t *next_block = NEXT_BLOCK(element_to_free);

    printf("Current element : %d; next element : %d\n", GET_BLOCK_SIZE(element_to_free), GET_BLOCK_SIZE(next_block));
    printf("Ocuppied : %d\n", IS_BLOCK_OCUPPIED(next_block));
    if(!(IS_BLOCK_OCUPPIED(next_block))) {
        MERGE_BLOCKS(element_to_free, next_block);
    }
    else {
        INITIALIZE_BLOCK(element_to_free, GET_BLOCK_SIZE(element_to_free));
    }

    printf("Block size : %d ; Ocuppied : %d\n", GET_BLOCK_SIZE(element_to_free), IS_BLOCK_OCUPPIED(element_to_free));
}


