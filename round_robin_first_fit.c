
#include "round_robin_first_fit.h"


void round_robin_first_fit(queue_t *queue, int quantum, int num_processes, memory_entry_t *memory_list) {
    int clock = 0;
    int total_turnaround_time = 0;
    int num_finished_processes = 0;
    double max_time_overhead = 0.0;
    double total_time_overhead = 0.0;
    process_t *current_process = NULL;
    queue_t *ready_queue = init_queue();
    int total_memory_used = 0;

    while (num_finished_processes < num_processes) {

        // Process arrives, put to ready queue
        if (!is_empty(queue) && queue -> head -> process_data -> arrival_time <= clock) {
            // If multiple processess arrived keep enqueuing
            while (!is_empty(queue) && queue -> head -> process_data -> arrival_time <= clock) {
                enqueue(ready_queue, dequeue(queue));
            }
        }
        
        // If the process has completed, dequeue, calculate turnaround time and time overhead
        if (current_process != NULL && current_process->remaining_time <= 0) {
            int turnaround_time = clock - current_process->arrival_time;

            total_turnaround_time += turnaround_time;
            num_finished_processes++;
            printf("%d,FINISHED,process-name=%s,proc-remaining=%d\n", clock, current_process->id, ready_queue->size);

            double time_overhead = (double)turnaround_time / current_process->service_time;
            total_time_overhead += time_overhead;
            if (time_overhead > max_time_overhead) {
                max_time_overhead = time_overhead;
            }

            deallocate_memory(&memory_list, current_process->allocated_address);
            total_memory_used -= current_process->memory_required;
            free(current_process->id);
            free(current_process);
            current_process = NULL;   
        }

        // No process is running and there are processes in the ready queue
        if (current_process == NULL && !is_empty(ready_queue)) {
            int num_processes_checked = 0;
            int num_processes_in_ready = ready_queue->size;

            while (!is_empty(ready_queue) && num_processes_checked < num_processes_in_ready) {

                // Check if process is already allocated memory
                if (ready_queue->head->process_data->allocated_address == -1) {

                    // Attempts to allocate memory to ready process, else move to tail of queue
                    if (allocate_memory(&memory_list, ready_queue->head->process_data->memory_required, &ready_queue->head->process_data->allocated_address, total_memory_used) == true) {
                        total_memory_used += ready_queue->head->process_data->memory_required;
                        current_process = dequeue(ready_queue);
                        clock = (clock < current_process->arrival_time) ? current_process->arrival_time : clock;
                        break;
                    } else {
                        enqueue(queue, dequeue(ready_queue));
                        num_processes_checked++;
                    }

                } else {
                    current_process = dequeue(ready_queue);
                    clock = (clock < current_process->arrival_time) ? current_process->arrival_time : clock;
                    break;
                }
            }
            printf("%d,RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%c,allocated-at=%d\n", clock, current_process->id, current_process->remaining_time, 
                (int)ceil(((double)total_memory_used / MAX_MEMORY * 100)), '%', current_process->allocated_address);

        // If the process has not completed and there are other processes in the ready queue
        } else if (current_process != NULL && !is_empty(ready_queue)) {
            process_t *next_process = ready_queue->head->process_data;
            int num_processes_checked = 0;
            int num_processes_in_ready = ready_queue->size;
            while(!is_empty(ready_queue) && num_processes_checked < num_processes_in_ready) {
                if (next_process->allocated_address == -1) {
                    if (allocate_memory(&memory_list, next_process->memory_required, &next_process->allocated_address, total_memory_used) == true) {
                        total_memory_used += next_process->memory_required;
                        enqueue(ready_queue, current_process);
                        current_process = dequeue(ready_queue);
                        break;
                    } else {
                        enqueue(queue, dequeue(ready_queue));
                        num_processes_checked++;

                        if (!is_empty(ready_queue)) {
                            next_process = ready_queue->head->process_data;
                        } else {
                            break;
                        }
                    }
                } else {
                    enqueue(ready_queue, current_process);
                    current_process = dequeue(ready_queue);
                    break;
                }
            }
            printf("%d,RUNNING,process-name=%s,remaining-time=%d,mem-usage=%d%c,allocated-at=%d\n", clock, current_process->id, current_process->remaining_time, 
                (int)((double)total_memory_used / MAX_MEMORY * 100 + 0.5), '%', current_process->allocated_address);
        }

        if (current_process != NULL) {
            current_process->remaining_time -= quantum;
        }
        
        // Stop the clock when all processes are finished
        if (num_finished_processes == num_processes) {
            break;
        }

        clock += quantum;
    }

    // Calculate and print performance statistics
    double average_turnaround_time = (double)total_turnaround_time / num_finished_processes;
    average_turnaround_time = ceil(average_turnaround_time); // Round up to the nearest integer

    printf("Turnaround time %d\n", (int)average_turnaround_time);

    double avg_time_overhead = total_time_overhead / num_finished_processes;
    avg_time_overhead = roundf(avg_time_overhead * 100) / 100; // Round to two decimal places

    printf("Time overhead %.2f %.2f\n", max_time_overhead, avg_time_overhead);

    printf("Makespan %d\n", clock);

    free_queue(ready_queue);
}