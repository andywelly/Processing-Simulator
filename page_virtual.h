
#ifndef PAGE_VIRTUAL_H
#define PAGE_VIRTUAL_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "queue.h"
#include "page.h"

#define MIN_PAGES_REQUIRED 4

// Allocates pages to frames with num_required_pages being the maximum pages needed
void allocate_pages_virtual(page_array_t* page_array, process_t* process, int num_required_pages, int clock);

// Allocates pages to frames with the number of pages required to allocate
void allocate_num_pages(page_array_t *page_array, process_t* process, int num_to_allocate);

// Deallocates number of pages neeeded and prints which frames evicted
void deallocate_virtual_pages(page_array_t* page_array, process_t* process, int num_to_deallocate, int clock);

// Similar to pages_to_string in page.h, however need to specify number of pages to print
char* select_pages_to_string(int* pages, int num_pages);

#endif


