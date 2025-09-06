
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#ifndef __SMT_plot_func_header
#define __SMT_plot_func_header


void plot_update_cpu_utilization(double cpu_utilization_percentage,
                                 int indexes[], int sample);
    ///_|> descry: this function plot current sample of cpu utilization of the
    //system on the graph
    ///_|>  int indexes[3]: this indicates the array which will store the reference points for the graphs
    // so at index 0 its for memory graph at index 1 its for cpu graph and at index 2 its for num of cores.
    ///_|> int samples: this indicates the current sample
    ///_|>  returns: this function returns nothing

void plot_update_memory_utilization(double mem_utilization_gbs, int indexes[],
                                    int sample);
    ///_|> descry: this function plot current sample of memeory utilization of the
    //system on the graph
    ///_|>  int indexes[3]: this indicates the array which will store the reference points for the graphs
    // so at index 0 its for memory graph at index 1 its for cpu graph and at index 2 its for num of cores.
    ///_|> int samples: this indicates the current sample
    ///_|>  returns: this function returns nothing
// void populate_graphs_concurrent(int system_monitoring_flags[3], int samples, int tdelay, int indexes[3]);
//     ///_|> descry: this function plot current sample of cpu utilization and memeory utilization of the
//     //system on the graph concurrently
//     ///_|>  int indexes[3]: this indicates the array which will store the reference points for the graphs
//     // so at index 0 its for memory graph at index 1 its for cpu graph and at index 2 its for num of cores.
//     ///_|> int samples: this indicates the current sample
//     ///_|> int tdelay: this indicates the delay between each sample collection in seconds.
//     ///_|>  returns: this function returns nothing
void system_monitoring_tool_label_generator(int system_monitoring_flags[3],
                                            int samples, int tdelay,
                                            int indexes[3]);
    ///_|> descry: this function prints all the labels for cpu and memeory graphs to the terminal
    ///_|>  int indexes[3]: this indicates the array which will store the reference points for the graphs
    // so at index 0 its for memory graph at index 1 its for cpu graph and at index 2 its for num of cores.
    ///_|> int samples: this indicates the total number of samples
    ///_|> int tdelay: this indicates the delay between each sample collection in seconds.
    ///_|>  returns: this function returns nothing

void graph_generator(int system_monitoring_flags[3], int indexes[3],
                     int samples, int *final_cursor_position);
    ///_|> descry: this function prints all the x and y axises  for cpu and memeory graphs to the terminal
    // and their max and min values
    ///_|>  int indexes[3]: this indicates the array which will store the reference points for the graphs
    // so at index 0 its for memory graph at index 1 its for cpu graph and at index 2 its for num of cores.
    ///_|> int samples: this indicates the total number of samples
    ///_|> int *final_cursor_position: this indicates value which will be used to determine the final termianl
    //position at the end of the program.
    ///_|>  returns: this function returns nothing
void generate_cores(int indexes[3], int *final_cursor_position, int num_squares, double max_freq); 
    ///_|> descry: this function prints grid of squares representing cpu cores.
    ///_|>  int indexes[3]: this indicates the array which will store the reference points for the graphs
    // so at index 0 its for memory graph at index 1 its for cpu graph and at index 2 its for num of cores.
    ///_|> int *final_cursor_position: this indicates value which will be used to determine the final termianl
    //position at the end of the program.
    ///_|>  returns: this function returns nothing                       
#endif