
#include "page.h"


// Initialise page_array and least recently used queue stored
page_array_t *init_array() {

    page_array_t* page_array = (page_array_t*)malloc(sizeof(page_array_t));
    assert(page_array);

    // Set all values of page array to empty (0)
    for (int i = 0; i < NUM_PAGES; i++) {
        page_array -> pages[i] = 0;
    }
    
    page_array -> num_available_pages = NUM_PAGES;
    page_array -> lru_queue = init_queue();
    return page_array;
}

// Allocates frames for a process to run
void allocate_frames(page_array_t* page_array, process_t* process, int num_required_pages, int clock) {
    
    // deallocate least recent used process if not enough frames available
    if (page_array -> num_available_pages < num_required_pages) {
        deallocate_lru(page_array, clock);
    }

    process -> allocated_pages = (int*)malloc(num_required_pages * sizeof(int));
    assert(process -> allocated_pages);

    // Update which frames are free in page_array and where process is allocated to
    int found_pages = 0;
    for (int i = 0; i < NUM_PAGES && found_pages < num_required_pages; i ++) {

        if (page_array -> pages[i] == 0) {
            page_array -> pages[i] = 1;
            process -> allocated_pages[found_pages++] = i;
        }
    }

    // If successfully allocated necessay number of pages
    if (found_pages == num_required_pages) {
        page_array -> num_available_pages -= num_required_pages;
        process -> num_allocated_pages += num_required_pages;


    } else {
        printf("Failed to Allocate");
    }

}

// Deallocates all frames of a process
void deallocate_frames(page_array_t* page_array, int clock, process_t *current_process) {
    for (int i = 0; i < current_process -> num_allocated_pages; i++) {
        int page_index = current_process -> allocated_pages[i];
        page_array -> pages[page_index] = 0;
        page_array -> num_available_pages++;
    }

    char* pages_string = pages_to_string(current_process);
    printf("%d,EVICTED,evicted-frames=%s\n",clock, pages_string);
    current_process -> num_allocated_pages = 0;

    free(current_process -> allocated_pages);    
    free(pages_string);

}

// Deallocate least recently used process
void deallocate_lru(page_array_t* page_array, int clock) {
    process_t* temp = dequeue(page_array -> lru_queue);

    // Iterate through all pages of process in head of lru queue and deallocate
    for (int i = 0; i < temp -> num_allocated_pages; i++) {
        int page_index = temp -> allocated_pages[i];
        page_array -> pages[page_index] = 0;
        page_array -> num_available_pages++;
    }

    char* pages_string = pages_to_string(temp);
    printf("%d,EVICTED,evicted-frames=%s\n",clock, pages_string);
    free(temp -> allocated_pages);
    free(pages_string);
    temp -> num_allocated_pages = 0;
}

// Converts frames to a string
char* pages_to_string(process_t* process) {
    if (process == NULL || process -> num_allocated_pages == 0) {
        return "empty";
    }

    int required_length = process -> num_allocated_pages * (CHARS_PER_PAGE);
    char *result = (char*)malloc(required_length);
    assert(result);

    result[0] = '[';
    result[1] = '\0';
    
    for (int i = 0 ; i < process -> num_allocated_pages; i++) {
        char buffer[CHARS_PER_PAGE + 1];
        if (i < process -> num_allocated_pages - 1) {
            sprintf(buffer, "%d,", process -> allocated_pages[i]);
        } else {
            sprintf(buffer, "%d", process -> allocated_pages[i]);
        }
        strcat(result, buffer);
    }
    strcat(result, "]");

    return result;
}

void free_array(page_array_t* page_array) {
    free_queue(page_array -> lru_queue);
    free(page_array);
}