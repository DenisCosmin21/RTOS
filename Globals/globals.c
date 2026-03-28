//
// Created by Denis on 11/23/2025.
//

#include "globals.h"

TCB_t *running_task = 0x00;
TCB_t *next_task = 0x00;
short started = 0;
int current_time = 0;
TCB_t *internal_idle_task = 0x00;