#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "SMT_structs.h"


#ifndef __SMT_fetch_data_header
#define __SMT_fetch_data_header

double fetch_proc_meminfo_categor_value_in_gb(char *category);
///_|> descry: this function fetches the corresponding value in the given 
  // categegory category in /proc/meminfo and returns it in GB
  ///_|> char * category: indicates the category we want to serach for the data from in /proc/meminfo
  ///_|>  returns: the function returns a double value of the specified memory category in GB or
  //exits(1) if not found and prints error message 
  
double fetch_memory_utilization_instance();
///_|> descry: this function fetches the current memeory utilization in gigabytes
  ///_|>  returns: the function returns a double of the current memory utilization in gigabytes

  
double get_total_memory_gb();
///_|> descry: this function gets the total memory available in the system in gigabytes.
  ///_|>  returns: the function returns a double memory available in the system in gigabytes

void fetch_cpu_info(CPU_INFO *instance);
  ///_|> descry: this function gets the total cpu utilization time and idle time form the system /proc/stat
  // and stores it  CPU_INFO struct.
  ///_|> CPU_INFO *instance: is the instance of the CPU_INFO struct we will updates its cpu time and idle fields
  // with the new and current cpu utilization time and idle time 
  ///_|>  returns: the function return nothing but the input struct has been
  // upadted with e new and current cpu utilization time and idle time 

double calc_cpu_percentage(CPU_INFO *instance, long prev_sum,long prev_idle);
///_|> descry: this function  function computes the percentage of CPU usage based on the
  // difference in total CPU time and idle CPU time between consecutive readings using formula 
  //given in class my math is the same if expanded and rearranged.
  ///_|> CPU_INFO *instance: is the instance of the CPU_INFO struct that contains
  //the most recent cpu util time and idle time
  ///_|> long prev_sum: indicates the previous cpu util time
  ///_|>  long prev_idle: indicates the previous idle time
  ///_|>  returns: the function return double of calculated current cpu percentage 
double fetch_max_frequency_in_GHz();
  ///_|> descry: this function gets the maximum cpu frequency in GHz from 
  //"/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq".
  ///_|>  returns: the function returns double of the max frequency of the cpu in GHz or prints error
  //and exits

int fetch_num_cores();
///_|> descry: this function gets the maximum cpu frequency in GHz from 
  //"/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq".
  ///_|>  returns: the function returns an int of the number of cores of the cpu


  

#endif 