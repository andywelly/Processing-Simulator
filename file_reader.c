
#include "file_reader.h"

// Reads process data file and adds all procesess to a queue
int read_file(const char *filename, queue_t **queue) {

    FILE *file = fopen(filename, "r");
    
    // If file cannot be read
    if (file == NULL) {
        fprintf(stderr, "File read failure\n");
        exit(EXIT_FAILURE);
    }

    int num_processes = 0;

    while (1) {
        process_t *current = (process_t*)malloc(sizeof(process_t));
        assert(current);

        // Allocate memory for id String
        current->id = (char*)malloc(MAX_ID_LEN * sizeof(char));
        assert(current->id);

        // read data into process information and setup other helper variables
        if (fscanf(file, "%d %s %d %d",
                   &current->arrival_time,
                   current->id,
                   &current->service_time,
                   &current->memory_required) == 4) {
            current->remaining_time = current->service_time;
            current->allocated_address = -1;
            current ->allocated_pages = NULL;
            current -> num_allocated_pages = 0;
            enqueue(*queue, current);
            num_processes++;
        } else { // reached end of file
            free(current->id);
            free(current);
            break;
        }
    }

    fclose(file);
    return num_processes;
}