#ifndef ROUND_ROBIN_INFINITE_H
#define ROUND_ROBIN_INFINITE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "queue.h"

// Completes infinite memory allocation round robin process simulation
void round_robin_infinite(queue_t *queue, int quantum, int num_processes);

#endif