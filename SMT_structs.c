#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "SMT_structs.h"

CPU_INFO *create_Cpu_Info(long int last_idle, long int previous_cpu_sum) {
  ///_|> descry: this function prints grid of squares representing cpu cores.
  ///_|>  long int last_idle: this indicates idle time of the cpu util time
  ///_|> long int previous_cpu_sum: this indicates the cpu util time 
  ///_|>  returns: this function struct CPU_INFO * with the fields updated with 
  //last_idle and previous_cpu_sum
  CPU_INFO *new_Cpu_Info = (CPU_INFO *)malloc(sizeof(CPU_INFO));
  new_Cpu_Info->idle = last_idle;
  new_Cpu_Info->cpu_sum = previous_cpu_sum;
  return new_Cpu_Info;
}