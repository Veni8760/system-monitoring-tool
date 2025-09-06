#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "SMT_structs.h"
#include "SMT_fetch_data.h"
#include "helper_func.h"
#include <sys/wait.h>

void plot_update_cpu_utilization(double cpu_utilization_percentage,
                                 int indexes[], int sample) {
  ///_|> descry: this function plot current sample of cpu utilization of the
  //system on the graph
  ///_|>  int indexes[3]: this indicates the array which will store the reference points for the graphs
  // so at index 0 its for memory graph at index 1 its for cpu graph and at index 2 its for num of cores.
  ///_|> int samples: this indicates the current sample
  ///_|>  returns: this function returns nothing

  if (cpu_utilization_percentage >= 0 && cpu_utilization_percentage <= 100) {
    int x_axis = indexes[1] + 10;
    int y_values_offset_right = 9;
    int index;

    terminal_cursor_mover(indexes[1], 8);
    printf("%.2f %%\n", cpu_utilization_percentage);

    if (cpu_utilization_percentage == 0) {
      index = 0;
    } else if (cpu_utilization_percentage > 0 &&
               cpu_utilization_percentage <= 10) {
      index = 1;
    } else if (cpu_utilization_percentage > 10 &&
               cpu_utilization_percentage <= 20) {
      index = 2;
    } else if (cpu_utilization_percentage > 20 &&
               cpu_utilization_percentage <= 30) {
      index = 3;
    } else if (cpu_utilization_percentage > 30 &&
               cpu_utilization_percentage <= 40) {
      index = 4;
    } else if (cpu_utilization_percentage > 40 &&
               cpu_utilization_percentage <= 50) {
      index = 5;
    } else if (cpu_utilization_percentage > 50 &&
               cpu_utilization_percentage <= 60) {
      index = 6;
    } else if (cpu_utilization_percentage > 60 &&
               cpu_utilization_percentage <= 70) {
      index = 7;
    } else if (cpu_utilization_percentage > 70 &&
               cpu_utilization_percentage <= 80) {
      index = 8;
    } else if (cpu_utilization_percentage > 80 &&
               cpu_utilization_percentage <= 90) {
      index = 9;
    } else if (cpu_utilization_percentage > 90 &&
               cpu_utilization_percentage <= 100) {
      index = 10;
    }
    // Move cursor and plot point without clearing the screen
    terminal_cursor_mover(x_axis - index + 1, y_values_offset_right + sample);
    printf(":\n");
    fflush(stdout);
  }
}

void plot_update_memory_utilization(double mem_utilization_gbs, int indexes[3],
                                    int sample) {
  ///_|> descry: this function plot current sample of memeory utilization of the
  //system on the graph
  ///_|>  int indexes[3]: this indicates the array which will store the reference points for the graphs
  // so at index 0 its for memory graph at index 1 its for cpu graph and at index 2 its for num of cores.
  ///_|> int samples: this indicates the current sample
  ///_|>  returns: this function returns nothing
  // constants for plotting
  int x_axis = indexes[0] + 13;
  int y_values_offset_right = 9;
  int intervals = 12;                             
  double total_memory_gbs = get_total_memory_gb(); // get total memory in GB
  double interval_value =
      total_memory_gbs / intervals; // we div total memory into 12 equal parts

  if (mem_utilization_gbs == 0) {
    terminal_cursor_mover(indexes[0], 11);
    printf("%.2f GB", mem_utilization_gbs);
    fflush(stdout);

    terminal_cursor_mover(x_axis, y_values_offset_right + sample);
    printf("#\n");
    fflush(stdout);
    return;
  }

  // loop through intervals for all other cases
  for (int i = 0; i < intervals; i++) {
    double interval_start = i * interval_value;
    double interval_end = (i + 1) * interval_value;

    // Handle memory usage within the current interval
    if (mem_utilization_gbs > interval_start &&
        mem_utilization_gbs <= interval_end) {
      terminal_cursor_mover(indexes[0], 11);
      printf("%.2f GB", mem_utilization_gbs);
      fflush(stdout);

      terminal_cursor_mover(x_axis - i, y_values_offset_right + sample);
      printf("#\n");
      fflush(stdout);

      break; 
    }
  }
}


void system_monitoring_tool_label_generator(int system_monitoring_flags[3],
                                            int samples, int tdelay,
                                            int indexes[3]) {
  ///_|> descry: this function prints all the labels and for cpu and memeory graphs to the terminal
  // and the general informtion passed by the user like the samples and tdelay
  ///_|>  int indexes[3]: this indicates the array which will store the reference points for the graphs
  // so at index 0 its for memory graph at index 1 its for cpu graph and at index 2 its for num of cores.
  ///_|> int samples: this indicates the total number of samples
  ///_|> int tdelay: this indicates the delay between each sample collection in seconds.
  ///_|>  returns: this function returns nothing

  double seconds = tdelay / 1000000.0;

  terminal_cursor_mover(1, 1);
  printf("Nbr of samples: %d -- every %d microseconds (%f secs)\n", samples,
         tdelay, seconds);
  fflush(stdout);

  // Memory Section
  if (system_monitoring_flags[0] == 1) {
    terminal_cursor_mover(indexes[0], 1);
    printf("v Memory\n");
    fflush(stdout);
  }

  // CPU Section
  if (system_monitoring_flags[1] == 1) {
    terminal_cursor_mover(indexes[1], 1);
    printf("v CPU\n");
    fflush(stdout);
  }
}

void graph_generator(int system_monitoring_flags[3], int indexes[3],
                     int samples, int *final_cursor_position) {
  ///_|> descry: this function prints all the x and y axises  for cpu and memeory graphs to the terminal
  // and their max and min values
  ///_|>  int indexes[3]: this indicates the array which will store the reference points for the graphs
  // so at index 0 its for memory graph at index 1 its for cpu graph and at index 2 its for num of cores.
  ///_|> int samples: this indicates the total number of samples
  ///_|> int *final_cursor_position: this indicates value which will be used to determine the final termianl
  //position at the end of the program.
  ///_|>  returns: this function returns nothing

  int memory_offset = 13;
  int cpu_offset = 11;
  int y_axis_offset = 8;
  int x_axis_offset = 8;

  // generate memory usage graph if the memory flag is 1
  if (system_monitoring_flags[0] == 1) {
    terminal_cursor_mover(indexes[0] + 1, 1);
    printf("%d GB\n", (int)get_total_memory_gb());
    fflush(stdout);

    // drawing the y-axis for memory usage
    for (int i = 0; i < 12; i++) {
      terminal_cursor_mover(indexes[0] + 1 + i, y_axis_offset);
      printf("|\n");
      fflush(stdout);
    }

    terminal_cursor_mover(indexes[0] + memory_offset, 2);
    printf("0 GB\n");
    fflush(stdout);

    // drawing the x-axis for memory usage
    for (int i = 0; i < samples + 1; i++) {
      terminal_cursor_mover(indexes[0] + memory_offset, x_axis_offset + i);
      printf("\u2500\n");
      fflush(stdout);
    }
    *final_cursor_position += indexes[0] + memory_offset;
  }

  //  cpuy  usage graph if the cpu flag is 1
  if (system_monitoring_flags[1] == 1) {
    terminal_cursor_mover(indexes[1] + 1, 3);
    printf("100%%\n");
    fflush(stdout);

    // drawinf the y-axis for cpu usage
    for (int i = 0; i < 10; i++) {
      terminal_cursor_mover(indexes[1] + 1 + i, y_axis_offset);
      printf("|\n");
      fflush(stdout);
    }

    //drawing the x-axis for cpu usage
    for (int i = 0; i < samples + 1; i++) {
      terminal_cursor_mover(indexes[1] + cpu_offset, x_axis_offset + i);
      printf("\u2500\n");
      fflush(stdout);
    }

    terminal_cursor_mover(indexes[1] + cpu_offset, 4);
    printf("0%%\n");
    fflush(stdout);
    if (system_monitoring_flags[0] == 1) {
      *final_cursor_position +=
          indexes[1] + cpu_offset - (indexes[0] + memory_offset);
    } else {
      *final_cursor_position += indexes[1] + cpu_offset;
    }
  }
}

void generate_cores(int indexes[3], int *final_cursor_position, int num_squares,double max_freq) {
  ///_|> descry: this function prints grid of squares representing cpu cores.
  ///_|>  int indexes[3]: this indicates the array which will store the reference points for the graphs
  // so at index 0 its for memory graph at index 1 its for cpu graph and at index 2 its for num of cores.
  ///_|> int *final_cursor_position: this indicates value which will be used to determine the final termianl
  //position at the end of the program.
  ///_|>  returns: this function returns nothing

  
  terminal_cursor_mover(indexes[2], 1);
  printf("v Number of Cores: %d @ %.2f GHz\n", num_squares,
          max_freq);
  fflush(stdout);

  terminal_cursor_mover(indexes[2] + 1, 1);
  int squares_per_row = 4; //max squares per row
  int rows =
      (num_squares + squares_per_row - 1) / squares_per_row; // calculating total rows
  *final_cursor_position += rows * 3 + 4;

  for (int r = 0; r < rows; r++) {
    int squares_in_current_row;

    // determine the number of squares in the current row
    //edge cases if number of squares is not multiple of 4
    if (r == rows - 1 && num_squares % squares_per_row != 0) {
      squares_in_current_row = num_squares % squares_per_row;
    } else {
      squares_in_current_row = squares_per_row;
    }

    // printing the top border of the squares
    for (int i = 0; i < squares_in_current_row; i++) {
      printf("  +───+ ");
      fflush(stdout);
    }
    printf("\n");
    fflush(stdout);

    // printing the sides of the squares
    for (int i = 0; i < squares_in_current_row; i++) {
      printf("  |   | ");
      fflush(stdout);
    }
    printf("\n");
    fflush(stdout);

    // printing the bottom border of the squares
    for (int i = 0; i < squares_in_current_row; i++) {
      printf("  +───+ ");
      fflush(stdout);
    }
    printf("\n");
    fflush(stdout);
  }
}