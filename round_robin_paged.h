#ifndef ROUND_ROBIN_PAGED_H
#define ROUND_ROBIN_PAGED_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "queue.h"
#include "page.h"

// values given in project spec
#define MAX_MEMORY 2048
#define PAGE_SIZE 4

// Completes paged memory allocation round robin process simulation
void round_robin_paged(queue_t *queue, int quantum, int num_processes);

#endif