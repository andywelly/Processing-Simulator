#ifndef PAGE_H
#define PAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "queue.h"

#define MAX_MEMORY 2048
#define PAGE_SIZE 4
#define NUM_PAGES (MAX_MEMORY / PAGE_SIZE)
#define CHARS_PER_PAGE 8 // Characters needed to print each page

// struct that stores available frames
typedef struct page_array {
    int num_available_pages;
    int pages[NUM_PAGES];
    queue_t* lru_queue;
} page_array_t;

// Initialise page_array and least recently used queue stored
page_array_t* init_array();

// Allocates frames for a process to run
void allocate_frames(page_array_t* page_array, process_t* process, int num_required_pages, int clock);

// Deallocates pages of least recently used process if memory needed 
void deallocate_lru(page_array_t* page_array, int clock);

// Deallocates all pages of a process when it completes running
void deallocate_frames(page_array_t* page_array, int clock, process_t *current_process);

// Returns the frames allocated to a process as a string
char* pages_to_string(process_t* process);

// Free page_array and inner malloced variables
void free_array(page_array_t* page_array);

#endif