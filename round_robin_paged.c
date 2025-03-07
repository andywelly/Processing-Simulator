
#include "round_robin_paged.h"


// Completes paged memory allocation round robin process simulation
void round_robin_paged(queue_t *queue, int quantum, int num_processes) {

    // Initialise variables;
    int clock = 0;
    int total_turnaround_time = 0;
    int num_finished_processes = 0;
    double max_time_overhead = 0.0;
    double total_time_overhead = 0.0;
    process_t *current_process = NULL;
    queue_t *ready_queue = init_queue();
    int pages_used;

    page_array_t *page_array = init_array();


    // While not all processess are complete
    while (num_finished_processes < num_processes) {
        
        // Process arrives, put to ready queue
        if (!is_empty(queue) && queue -> head -> process_data -> arrival_time <= clock) {
            while (!is_empty(queue) && queue -> head -> process_data -> arrival_time <= clock) {
                enqueue(ready_queue, dequeue(queue));
            }
        }

        // If process complete, dequeue and calculate turnaround time and overhead
        if (current_process != NULL && current_process -> remaining_time <= 0) {
            
            int turnaround_time = clock - current_process -> arrival_time;
            total_turnaround_time += turnaround_time;
            num_finished_processes++;
            
            // Removes all allocated frames from given process
            deallocate_frames(page_array, clock, current_process);
            printf("%d,FINISHED,process-name=%s,proc-remaining=%d\n", clock, current_process->id, ready_queue->size);

            // Update statistics
            double time_overhead = (double)turnaround_time / current_process -> service_time;
            total_time_overhead += time_overhead;
            if (time_overhead > max_time_overhead) {
                max_time_overhead = time_overhead;
            }

            // Free necessary mallocs
            free(current_process -> id);
            free(current_process);
            current_process = NULL;

        }

        // No process is running and processes are waiting, head process runs once
        if (current_process == NULL && !is_empty(ready_queue)) {
            current_process = dequeue(ready_queue);
            int num_required_pages = ceil((double)current_process -> memory_required / PAGE_SIZE);

            // If not allocated
            if (current_process -> num_allocated_pages != num_required_pages) {
                allocate_frames(page_array, current_process, num_required_pages, clock);
            }

            // Removes process of least recently used queue wherever it may be and enqueues
            remove_from_queue(page_array -> lru_queue, current_process);
            enqueue(page_array -> lru_queue, current_process);


            // Calculate performance statistics
            clock = (clock < current_process -> arrival_time) ? current_process -> arrival_time : clock;
            pages_used = NUM_PAGES - page_array -> num_available_pages;
            char *pages = pages_to_string(current_process);
            printf("%d,RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%c,mem-frames=%s\n", clock, current_process->id, current_process->remaining_time, 
                (int)ceil(((double)pages_used / NUM_PAGES * 100)), '%', pages);
            free(pages);

        // A process was running, and other processes waiting, next process runs once
        } else if (current_process != NULL && !is_empty(ready_queue)) {
            process_t *next_process = ready_queue -> head -> process_data;

            if (next_process -> arrival_time <= clock) {
                enqueue(ready_queue, current_process);
                current_process = dequeue(ready_queue);
                int num_required_pages = ceil((double)current_process -> memory_required / PAGE_SIZE);

                // If not allocated
                if (current_process -> num_allocated_pages != num_required_pages) {
                    allocate_frames(page_array, current_process, num_required_pages, clock);

                } // else if (already allocated) it just runs
                remove_from_queue(page_array -> lru_queue, current_process);
            enqueue(page_array -> lru_queue, current_process);

            }

            // Calculate performance statistics
            pages_used = NUM_PAGES - page_array -> num_available_pages;
            char *pages = pages_to_string(current_process);
            printf("%d,RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%c,mem-frames=%s\n", clock, current_process->id, current_process->remaining_time, 
                (int)ceil(((double)pages_used/ NUM_PAGES * 100)), '%', pages);
            free(pages);
        }

        // If process was already running and no new processess
        if (current_process != NULL) {
            current_process -> remaining_time -= quantum;
        }

        // Second check if all processess have finished
        if (num_finished_processes == num_processes) {
            break;
        }

        clock += quantum;
    }

    // Final calculation and print performance statistics
    double average_turnaround_time = (double)total_turnaround_time / num_finished_processes;
    average_turnaround_time = ceil(average_turnaround_time); // Round up to the nearest integer

    printf("Turnaround time %d\n", (int)average_turnaround_time);

    double avg_time_overhead = total_time_overhead / num_finished_processes;
    avg_time_overhead = roundf(avg_time_overhead * 100) / 100; // Round to two decimal places

    printf("Time overhead %.2f %.2f\n", max_time_overhead, avg_time_overhead);
    printf("Makespan %d\n", clock);


    // Free necessary values
    free_queue(ready_queue);
    free_array(page_array);


}
