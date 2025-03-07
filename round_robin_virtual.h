#ifndef ROUND_ROBIN_VIRTUAL_H
#define ROUND_ROBIN_VIRTUAL_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "queue.h"
#include "page.h"
#include "page_virtual.h"

// Completes virtual memory allocation round robin process simulation
void round_robin_virtual(queue_t *queue, int quantum, int num_processes);

#endif
