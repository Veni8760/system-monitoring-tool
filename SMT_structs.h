/*
 *  Header file for our structs of the System Monitoring Tool.
 *
 *  Author: Daniel Venistan.
 * 
 * Important!: format is from B63 assignment header files I am using it to document the header file!
 */

 #include <ctype.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <unistd.h>
 
  
 #ifndef __SMT_structs_header
 #define __SMT_structs_header
 
 
 typedef struct cpu_times {
    long int idle;   
    long int cpu_sum; 
  } CPU_INFO;
  
  CPU_INFO *create_Cpu_Info(long int last_idle, long int previous_cpu_sum);
    ///_|> descry: this function prints grid of squares representing cpu cores.
    ///_|>  long int last_idle: this indicates idle time of the cpu util time
    ///_|> long int previous_cpu_sum: this indicates the cpu util time 
    ///_|>  returns: this function struct CPU_INFO * with the fields updated with 
    //last_idle and previous_cpu_sum


 #endif