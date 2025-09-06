#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "SMT_fetch_data.h"
#include "SMT_structs.h"
#include "helper_func.h"



double fetch_proc_meminfo_categor_value_in_gb(char *category) {
  ///_|> descry: this function fetches the corresponding value in the given 
  // categegory category in /proc/meminfo and returns it in GB
  ///_|> char * category: indicates the category we want to serach for the data from in /proc/meminfo
  ///_|>  returns: the function returns a double value of the specified memory category in GB or
  //exits(1) if not found and prints error message 

  char current_string[256];
  FILE *fptr = fopen("/proc/meminfo", "r");

    if (fptr == NULL) {
        perror("Error opening /proc/meminfo");
        exit(1);
    }


  while (fgets(current_string, sizeof(current_string), fptr)) {
    //Split the string by category and value
    char *string = strtok(current_string, ":");
    //Check if the category is the one we desire
    if (string != NULL && strcmp(string, category) == 0) {
      //fetch the corresponding value for the category
      char *value_str = strtok(NULL, " "); 
      if (value_str != NULL) {
        //convert to long long as number can be large
        long long value = strtoll(value_str, NULL, 10);
        fclose(fptr);           
        //convert value into kbs to gbs and return
        return kb_to_gb(value); 
      } 
    }
  }

  //we never found the category
  fclose(fptr);
  fprintf(stderr, "Error: Category %s not found in /proc/meminfo\n", category);
  exit(1);
}
  
double fetch_memory_utilization_instance() {
  ///_|> descry: this function fetches the current memeory utilization in gigabytes
  ///_|>  returns: the function returns a double of the current memory utilization in gigabytes

  // fetch the total memory avaiable in the system
  double memory_total = fetch_proc_meminfo_categor_value_in_gb("MemTotal");

  //fetch the avaiable memeory in the system
  double memory_available =
      fetch_proc_meminfo_categor_value_in_gb("MemAvailable");

  //If they are somehow negative somthing went wrong so exit and print error message
  if (memory_total <= -1 || memory_available <= -1) {
    printf("\033[2J");
    fprintf(stderr,"Error reading memory information.\n");
    exit(1);
  }
 
  //calc memory utilization
  double memory_used = memory_total - memory_available;

  return memory_used;
}

double get_total_memory_gb() {
  ///_|> descry: this function gets the total memory available in the system in gigabytes.
  ///_|>  returns: the function returns a double memory available in the system in gigabytes
  return fetch_proc_meminfo_categor_value_in_gb("MemTotal");
}
  
void fetch_cpu_info(CPU_INFO *instance) {
  ///_|> descry: this function gets the total cpu utilization time and idle time form the system /proc/stat
  // and stores it  CPU_INFO struct.
  ///_|> CPU_INFO *instance: is the instance of the CPU_INFO struct we will updates its cpu time and idle fields
  // with the new and current cpu utilization time and idle time 
  ///_|>  returns: the function return nothing but the input struct has been
  // upadted with e new and current cpu utilization time and idle time 


  char *separator = " ";
  char cpu_line[256];
  long cpu_sum = 0;
  long idle = 0;

  FILE *fptr = fopen("/proc/stat", "r");
  if (fptr == NULL) {
      perror("Error opening /proc/stat");
      exit(1);
  }

  // Read the first line from /proc/stat
  if (fgets(cpu_line, sizeof(cpu_line), fptr) == NULL) {
      perror("Error reading /proc/stat");
      fclose(fptr);
      exit(1);
  }
  fclose(fptr);

  // tokenize to extract CPU times
  int column = 0;
  //skip the cpu string
  char *string = strtok(cpu_line, separator);
  //sum up  user + nice + sys + idle + IOwait + irq + softirq + steal +guest
  // the last two numbers are always zero  and we get store the idle time at the 4 column
  while ((string = strtok(NULL, separator)) != NULL) {
    long value = strtol(string, NULL, 10);
    cpu_sum += value;
    // column index 3 corresponds to the idle time
    if (column++ == 3) {
      idle = value;
    }
  }

  // Store the computed idle and total CPU time in the provided struct
  instance->idle = idle;
  instance->cpu_sum = cpu_sum;
}

double calc_cpu_percentage(CPU_INFO *instance, long prev_sum,long prev_idle) {
  ///_|> descry: this function  function computes the percentage of CPU usage based on the
  // difference in total CPU time and idle CPU time between consecutive readings using formula 
  //given in class my math is the same if expanded and rearranged.
  ///_|> CPU_INFO *instance: is the instance of the CPU_INFO struct that contains
  //the most recent cpu util time and idle time
  ///_|> long prev_sum: indicates the previous cpu util time
  ///_|>  long prev_idle: indicates the previous idle time
  ///_|>  returns: the function return double of calculated current cpu percentage 

  long previous_sum = prev_sum;
  long previous_idle = prev_idle;

  long total_delta = instance->cpu_sum - previous_sum;
  if (total_delta <= 0) {
    return 0.0;
  }

  long idle_delta = instance->idle - previous_idle;
  return 100.0 * (1.0 - ((double)idle_delta / total_delta));
}
  
double fetch_max_frequency_in_GHz() {
  ///_|> descry: this function gets the maximum CPU frequency in GHz from 
  // "/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq".
  ///_|> returns: the function returns the max frequency of the CPU in GHz or prints an error and exits.
  double max_frequency = 0;

  FILE *fptr = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq", "r");
  if (fptr == NULL) {
      perror("Error opening /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq");
      exit(1);
  }
  
  if (fscanf(fptr, "%lf", &max_frequency) != 1) {
      perror("Failed to read the value from /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq");
      fclose(fptr);
      exit(1);
  }
  fclose(fptr);

  // Convert the max frequency in kHz to GHz
  return kHz_to_GHz(max_frequency);
}



int fetch_num_cores() {
  ///_|> descry: this function gets the maximum cpu frequency in GHz from 
  //"/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq".
  ///_|>  returns: the function returns an int of the number of cores of the cpu
  char line[256];
  char first_word[256];
  const char *word = "processor";
  int num_cores = 0;
  FILE *fptr = fopen("/proc/cpuinfo", "r");


  if (fptr == NULL) {
    printf("\033[2J");
    perror("Error opening /proc/cpuinfo");
    exit(1);
    return -1;
  }

  while (fgets(line, sizeof(line), fptr)) {
    sscanf(line, "%s", first_word);
    if (strcmp(first_word, word) == 0) { 
      num_cores++;
    }
  }

  fclose(fptr);    
  return num_cores; 
}