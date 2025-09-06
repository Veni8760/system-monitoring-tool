#define _DEFAULT_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "SMT_Logic.h"
#include <signal.h>
#include "helper_func.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "SMT_structs.h"
#include "SMT_fetch_data.h"
#include "helper_func.h"
#include <sys/wait.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "SMT_Logic.h"
#include "SMT_plot_func.h"
#include "helper_func.h"
#include "SMT_fetch_data.h"
#include <sys/wait.h>



static void catch_function(int signo) {
    char user_answer[10];
    terminal_cursor_mover(35, 0);
    printf("Do you want to quit or continue? (y,n): ");
    scanf(" %9s", user_answer);
  
    terminal_cursor_mover(35, 0);
    if (strcmp(user_answer, "y") == 0) {
        kill(0, SIGKILL);
    } else {
        terminal_cursor_mover(35, 0);
        printf("\033[2K\r");
        return;
    }
}



int main(int argc, char **argv) {

    if( signal(SIGINT , catch_function ) == SIG_ERR ) {
        fputs ( " An error occurred while setting a signal handler .\n " , stderr ) ;
    return EXIT_FAILURE ;
    }

    signal(SIGTSTP, SIG_IGN);

    int final_cursor_position = 0;
    int samples = 20;
    int tdelay = 500000;  
    int indexes[] = {0, 0, 0};
    int system_monitoring_flags[3] = {0, 0, 0};
    int num_of_cores = 0;
    double max_frequency = 0.0;
    double memory_util = 0.0;
    double cpu_util = 0.0;

    // Parse command line arguments and set up indexes.
    int result = parse_command_line_arguments(argc, argv, &samples, &tdelay, system_monitoring_flags);
    system_monitoring_tool_indexes(system_monitoring_flags, indexes);

    if (result == 0) {
    // Clear screen and move cursor to home
    printf("\033[2J");
    printf("\033[H");

    // generate labels and graph axes
    system_monitoring_tool_label_generator(system_monitoring_flags, samples, tdelay, indexes);
    graph_generator(system_monitoring_flags, indexes, samples, &final_cursor_position);

    
    int cores_pipe[2], freq_pipe[2];

    if (system_monitoring_flags[2] == 1) {
        // pipe and fork for num_of_cores
        if (pipe(cores_pipe) == -1) {
            perror("Failed to create pipe for cores");
            exit(1);
        }
        pid_t cores_pid = fork();
        if (cores_pid == -1) {
            perror("Fork failed for cores");
              exit(1);
        }
        if (cores_pid == 0) {
            close(cores_pipe[0]); // close unused read end in child
            int cores = fetch_num_cores();
            write(cores_pipe[1], &cores, sizeof(cores));
            close(cores_pipe[1]); // close write end after use
            exit(0);
        }

        // pipe and fork for frequency
        if (pipe(freq_pipe) == -1) {
            perror("Failed to create pipe for frequency");
            exit(1);
        }
        pid_t freq_pid = fork();
        if (freq_pid == -1) {
            perror("Fork failed for frequency");
            exit(1);
        }
        if (freq_pid == 0) {
            close(freq_pipe[0]); // close unused read end in child
            double freq = fetch_max_frequency_in_GHz();
            write(freq_pipe[1], &freq, sizeof(freq));
            close(freq_pipe[1]); // close write end after use
            exit(0);
        } 
    }

    if (system_monitoring_flags[0] == 1 || system_monitoring_flags[1] == 1) {
        CPU_INFO *cpu_info = create_Cpu_Info(0, 0);
        for (int i = 0; i < samples; i++) {
            int memory_data_pipe[2];
            int cpu_utilization_pipe[2];
            if (system_monitoring_flags[0] == 1) {
                
                if (pipe(memory_data_pipe) == -1) {
                    perror("Failed to create memory pipe");
                    exit(1);
                }
                pid_t memory_pid = fork();
                if (memory_pid == -1) {
                    perror("fork failed");
                    exit(1);
                }
                if (memory_pid == 0) { 
                    close(memory_data_pipe[0]); // close unused read end in child
                    double mem_val = fetch_memory_utilization_instance();
                    write(memory_data_pipe[1], &mem_val, sizeof(double));
                    close(memory_data_pipe[1]); // close write end after use
                    exit(0);
                }
            }

            if (system_monitoring_flags[1] == 1) {
                if (pipe(cpu_utilization_pipe) == -1) {
                    perror("Failed to create CPU pipe");
                    exit(1);
                }
                pid_t cpu_pid = fork();
                if (cpu_pid == -1) {
                    perror("fork failed");
                    exit(1);
                }
                if (cpu_pid == 0) {
                    close(cpu_utilization_pipe[0]); // close unused read end in child
                    CPU_INFO new_cpu_info;
                    fetch_cpu_info(&new_cpu_info);
                    write(cpu_utilization_pipe[1], &new_cpu_info, sizeof(CPU_INFO));
                    close(cpu_utilization_pipe[1]); // close write end after use
                    exit(0);
                }
            }

            if (system_monitoring_flags[0] == 1) {
                wait(NULL);
            }
            if (system_monitoring_flags[1] == 1) {
                wait(NULL);
            }

            if (system_monitoring_flags[0] == 1) {
                read(memory_data_pipe[0], &memory_util, sizeof(double));
                close(memory_data_pipe[0]); // Done reading
                plot_update_memory_utilization(memory_util, indexes, i);
            }

            if (system_monitoring_flags[1] == 1) {
                CPU_INFO new_cpu_info;
                read(cpu_utilization_pipe[0], &new_cpu_info, sizeof(CPU_INFO));
                close(cpu_utilization_pipe[0]); // Done reading
                cpu_util  = calc_cpu_percentage(&new_cpu_info, cpu_info->cpu_sum, cpu_info->idle);
                *cpu_info = new_cpu_info;
                plot_update_cpu_utilization(cpu_util, indexes, i);
            }
            usleep(tdelay);
        }
        free(cpu_info);
    } else {
        perror("Invalid result from parse_command_line_arguments");
        exit(1);
    }

    if (system_monitoring_flags[2] == 1) {
        wait(NULL);
        wait(NULL);
        read(cores_pipe[0], &num_of_cores, sizeof(num_of_cores));
        close(cores_pipe[0]);

        read(freq_pipe[0], &max_frequency, sizeof(max_frequency));
        close(freq_pipe[0]);

        generate_cores(indexes, &final_cursor_position, num_of_cores, max_frequency);
    }

    terminal_cursor_mover(final_cursor_position + 1, 1);
    } 
    else {
      perror("Invalid result from parse_command_line_arguments");
      exit(1);
  }
  return 0;
}