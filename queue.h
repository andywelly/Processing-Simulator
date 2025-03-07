#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_ID_LEN 9

// process struct to store all necessary information
typedef struct {
    char *id;
    int arrival_time;
    int service_time;
    int remaining_time;
    int memory_required;
    int allocated_address;
    int *allocated_pages;
    int num_allocated_pages;
} process_t;

// node struct used in process queues
typedef struct node {
    process_t *process_data;
    struct node *next;
} node_t;

// process queue struct
typedef struct queue {
    node_t *head;
    node_t *tail;
    int size;
} queue_t;

// Initializes a queue
queue_t* init_queue();

// Checks if the queue is empty
int is_empty(queue_t *queue);

// Adds a process to the queue
void enqueue(queue_t *queue, process_t *process_data);

// Removes a process from the queue head
process_t* dequeue(queue_t *queue);

// Removed a given process from queue
void remove_from_queue(queue_t *queue, process_t *process);

// Frees the queue
void free_queue(queue_t *queue);

#endif