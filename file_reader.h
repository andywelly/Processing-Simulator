#ifndef FILE_READER_H
#define FILE_READER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "queue.h"

// Reads process data file and adds all procesess to a queue
int read_file(const char *filename, queue_t **queue);

#endif