#ifndef ROUND_ROBIN_FIRST_FIT_H
#define ROUND_ROBIN_FIRST_FIT_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "queue.h"
#include "memory.h"

// values given in project spec
#define MAX_MEMORY 2048

// Completes first fit memory allocation round robin process simulation
void round_robin_first_fit(queue_t *queue, int quantum, int num_processes, memory_entry_t *memory_list);
#endif