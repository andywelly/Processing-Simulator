#include "page_virtual.h"

// Allocates pages to frames with num_required_pages being the maximum pages needed
void allocate_pages_virtual(page_array_t *page_array, process_t* process, int num_required_pages, int clock) {

    int num_to_allocate = num_required_pages - process -> num_allocated_pages;

    // If process requires less than 4 pages
    if (num_required_pages < MIN_PAGES_REQUIRED) {
        if (page_array -> num_available_pages < num_to_allocate) {

            // deallocate num_to_allocate pages
            deallocate_virtual_pages(page_array, process, num_to_allocate, clock);
        }
        allocate_num_pages(page_array, process, num_to_allocate);

    } else { // Process required 4 or mote pages

        // If enough room to allocate whole process
        if (page_array -> num_available_pages >= num_to_allocate) {
            allocate_num_pages(page_array, process, num_to_allocate);

        // If enough room to allocate at least 4 pages
        } else if (page_array -> num_available_pages >= MIN_PAGES_REQUIRED) {
            allocate_num_pages(page_array, process, page_array -> num_available_pages);
        
        // Else have to deallocate 4 pages (or less if some free at the end)
        } else {
            int num_to_deallocate = MIN_PAGES_REQUIRED - page_array -> num_available_pages;
            deallocate_virtual_pages(page_array, process, num_to_deallocate, clock);
            allocate_num_pages(page_array, process, MIN_PAGES_REQUIRED);
        }
    }


}

// Allocates pages to frames with the number of pages required to allocate
void allocate_num_pages(page_array_t* page_array, process_t* process, int num_to_allocate) {

    if (process -> allocated_pages == NULL) {
        process -> allocated_pages = (int*)malloc(num_to_allocate * sizeof(int));
        assert(process -> allocated_pages);
    } else {
        process -> allocated_pages = (int*)realloc(process -> allocated_pages,
                                            (process -> num_allocated_pages + num_to_allocate) * sizeof(int));
        assert(process -> allocated_pages);
    }


    // Start found_pages starts from num_allocated
    int found_pages = process -> num_allocated_pages;
    for (int i = 0; i < NUM_PAGES && found_pages < num_to_allocate + process -> num_allocated_pages; i++) {
        if (page_array -> pages[i] == 0) {
            page_array -> pages[i] = 1;
            process -> allocated_pages[found_pages++] = i;
        }
    }
    page_array -> num_available_pages -= num_to_allocate;
    process -> num_allocated_pages += num_to_allocate;
}

// Deallocates number of pages neeeded and prints which frames evicted
void deallocate_virtual_pages(page_array_t* page_array, process_t* process, int num_to_deallocate, int clock) {
    process_t* temp = NULL;


    int removed_all = 0; // Check if whole process removed

    // If all pages used by process deallocated, 
    if (page_array -> lru_queue -> head -> process_data -> num_allocated_pages <= num_to_deallocate) {
        temp = dequeue(page_array -> lru_queue);
        removed_all = 1;
    } else {
        // If not removed all dont dequeue from lru queue
        temp = page_array -> lru_queue -> head -> process_data;
    }

    // String to store evicted frames
    int *evicted = (int*)malloc(num_to_deallocate * sizeof(int));
    int num_deallocated = 0;

    // Get the frame numbers to deallocate
    for (int i = 0; i < temp -> num_allocated_pages && num_deallocated < num_to_deallocate; i++) {
        int page_index = temp -> allocated_pages[i];

        // if the frame is being used
        if (page_array -> pages[page_index] == 1) {
            page_array -> pages[page_index] = 0;
            page_array -> num_available_pages++;
            evicted[num_deallocated++] = page_index;
        } 

        // shift remaining elements
        for (int j = i; j < temp -> num_allocated_pages - 1; j++) {
            temp -> allocated_pages[j] = temp -> allocated_pages[j + 1];
        }
        temp -> num_allocated_pages--;
        i--;
    }

    // print result
    char* pages_string = select_pages_to_string(evicted, num_to_deallocate);
    printf("%d,EVICTED,evicted-frames=%s\n",clock, pages_string);
    free(pages_string);
    free(evicted);


    // If entire process was removed free
    if (removed_all == 1) {
        free(temp -> allocated_pages);
        temp -> allocated_pages = NULL;
    }

}



// Similar to pages_to_string, however need to specify number of pages to print
char* select_pages_to_string(int* pages, int num_pages) {
    int required_length = num_pages * (CHARS_PER_PAGE);
    char* result = (char*)malloc(required_length);
    assert(result);

    result[0] = '[';
    result[1] = '\0';

    for (int i = 0; i < num_pages; i++) {
        char buffer[CHARS_PER_PAGE + 1];
        // If not last value add ","
        if (i < num_pages - 1) {
            sprintf(buffer, "%d,", pages[i]);
        } else { // If last value no ","
            sprintf(buffer, "%d", pages[i]);
        }
        strcat(result, buffer);
    }
    strcat(result, "]");
    // Have to free result after printing
    return result;
}
