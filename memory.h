#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "queue.h"

#define MAX_MEMORY 2048

// struct to store which parts of memory are free (H) or allocated to a process (P)
typedef struct memory_entry {
    char type; // 'P' for process, 'H' for hole
    int address;
    int size;
    struct memory_entry *next;
} memory_entry_t;

// Allocates memory to process and updates memory list, returns true is sucess
bool allocate_memory(memory_entry_t **memory_list, int size, int *allocated_address, int total_memory_used);

// Deallocates memory from process and updates memoery list, returns true if sucess
bool deallocate_memory(memory_entry_t **memory_list, int address);




#endif