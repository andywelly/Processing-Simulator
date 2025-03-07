
#include "queue.h"

// Initializes a queue
queue_t* init_queue() {
    queue_t *queue = (queue_t*)malloc(sizeof(queue_t));
    assert(queue);
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    return queue;
}

// Checks if the queue is empty
int is_empty(queue_t *queue) {
    return queue->head == NULL;
}

// Adds a process to the queue
void enqueue(queue_t *queue, process_t *process) {
    
    // Create new node
    node_t *new_node = (node_t*)malloc(sizeof(node_t));
    assert(new_node);
    new_node->process_data = process;
    new_node->next = NULL;

    if (is_empty(queue)) {
        queue->head = new_node;
        queue->size = 1;
    } else {
        queue->tail->next = new_node;
        queue->size++;
    }

    queue->tail = new_node;
}

// Removes a process from the queue head
process_t* dequeue(queue_t *queue) {
    if (is_empty(queue)) {
        return NULL;
    }

    node_t *temp = queue->head;
    process_t *process = temp->process_data;
    queue->head = queue->head->next;
    queue->size--;

    // Check if node is the only item in queue
    if (queue->head == NULL) {
        queue->tail = NULL;
    }

    free(temp);
    return process;
}

// Removed a given process from queue
void remove_from_queue(queue_t *queue, process_t *process) {
    if (is_empty(queue)) {
        return;
    }
    node_t *current = queue -> head;
    node_t *prev = NULL;

    while (current != NULL) {

        // Found the process to remove
        if (current -> process_data == process) {
            
            // If process is at the head
            if (prev == NULL) {
                queue -> head = current -> next;

                // If process is empty
                if (queue -> head == NULL) {
                    queue -> tail = NULL;
                }
            } else { // Process in middle or end
                prev -> next = current -> next;

                // If process at end
                if (current -> next == NULL) {
                    queue -> tail = prev;
                }

            }
            free(current);
            queue -> size--;
            return;
        } 
        prev = current;
        current = current -> next;
    }
    

   
}

// Frees the queue and dequeues inner 
void free_queue(queue_t *queue) {
    while (!is_empty(queue)) {
        dequeue(queue);
    }
    free(queue);
}