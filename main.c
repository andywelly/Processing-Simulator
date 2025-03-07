#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "file_reader.h"
#include "queue.h"
#include "round_robin_first_fit.h"
#include "round_robin_infinite.h"
#include "round_robin_paged.h"
#include "round_robin_virtual.h"
#include "memory.h"

#define MAX_MEMORY 2048

int main(int argc, char const *argv[]) {
    const char *filename = NULL;
    const char *memory_strategy = NULL;
    int quantum = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            filename = argv[++i];
        } else if (strcmp(argv[i], "-m") == 0) {
            memory_strategy = argv[++i];
        } else if (strcmp(argv[i], "-q") == 0) {
            quantum = atoi(argv[++i]);
        }
    }

    queue_t *process_queue = init_queue();
    int num_processes = read_file(filename, &process_queue);

    // Initialize memory list with a single hole entry representing the entire available memory
    memory_entry_t *memory_list = (memory_entry_t *)malloc(sizeof(memory_entry_t));
    memory_list->type = 'H';
    memory_list->address = 0;
    memory_list->size = MAX_MEMORY;
    memory_list->next = NULL;

    // Run the appropriate memory strategy
    switch (memory_strategy[0]) {
        case 'i':
            round_robin_infinite(process_queue, quantum, num_processes);
            break;
        case 'f':
            round_robin_first_fit(process_queue, quantum, num_processes, memory_list);
            break;
        case 'p':
            round_robin_paged(process_queue, quantum, num_processes);
            break;
        case 'v':
            round_robin_virtual(process_queue, quantum, num_processes);
            break;
        default:
            fprintf(stderr, "Invalid memory strategy\n");
            exit(EXIT_FAILURE);
            break;
    }

    // Free memory list
    memory_entry_t *current = memory_list;
    while (current != NULL) {
        memory_entry_t *next = current->next;
        free(current);
        current = next;
    }

    free_queue(process_queue);

    return 0;
}