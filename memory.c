
#include "memory.h"

// Allocates memory to process and updates memory list, returns true is sucess
bool allocate_memory(memory_entry_t **memory_list, int size, int *allocated_address, int total_memory_used) {
    memory_entry_t *current = *memory_list;

    // Check if there is enough memory to allocate
    if (total_memory_used + size <= MAX_MEMORY) {
        current = *memory_list;

        // Iterate through each part of memory
        while (current != NULL) {

            // If section of memeory is unallocated (hole)
            if (current->type == 'H' && current->size >= size) {
                *allocated_address = current->address;
                current->type = 'P';
                int remaining_size = current->size - size;

                if (remaining_size > 0) {
                    // Create a new memory entry for the remaining hole
                    memory_entry_t *new_hole = (memory_entry_t *)malloc(sizeof(memory_entry_t));
                    new_hole->type = 'H';
                    new_hole->address = current->address + size;
                    new_hole->size = remaining_size;
                    new_hole->next = current->next;

                    // Update the current entry to represent the allocated process
                    current->size = size;
                    current->next = new_hole;
                }

                return true;
            }

            current = current->next;
        }
    }

    return false;
}


// Deallocates memory from process and updates memoery list, returns true if sucess
bool deallocate_memory(memory_entry_t **memory_list, int address) {
    memory_entry_t *current = *memory_list;
    memory_entry_t *prev = NULL;

    // Iterate through each part of memory
    while (current != NULL) {

        // If reached part of memory to deallocate
        if (current->address == address) {
            current->type = 'H';

            // Check if the next entry is a hole, if so, merge current hole with next hole
            if (current->next != NULL && current->next->type == 'H') {
                current->size += current->next->size;
                memory_entry_t *temp = current->next;
                current->next = current->next->next;
                free(temp);
            }

            // Check if the previous entry is a hole, if so, merge current hole with previous hole
            if (prev != NULL && prev->type == 'H') {
                prev->size += current->size;
                prev->next = current->next;
                free(current);
            }

            return true;
        }

        prev = current;
        current = current->next;
    }

    return false;
}
