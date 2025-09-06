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



int parse_command_line_arguments(int argc, char **argv, int *samples,
                                 int *tdelay, int system_monitoring_flags[3]) {
  ///_|> descry: this function parses the CLA to check for user input like 
  // num of smaples, delay in microseconds, the system monitoring graphs to show like
  // cpu, memeory,cores
  ///_|> int argc:  indicates the  number of CLA 
  ///_|> char **argv: indicates the  array of CLA string
  ///_|>  int *samples: indicates the poiter which will store the number of smaples
  ///_|> int *tdelay: indicates the pointer which will store the tdelay in microseconds
  ///_|> int system_monitoring_flags[3]: indicates the arrayo which stores the integer flags of the graphs we want to output
  // in the program 0 not to show 1 to show for cpu, memeory,cores graphs 
  ///_|>  returns: the function returns an int -1 for failure or 0 on successful parsing
  //valid CLA's

  // samples will indicate how many times we collect stats cpu or memoery we default 20
  int samples_flag = 0;
  // tdelay is frequency in microseconds the default 500000
  int tdelay_flag = 0;
  int propositional_arguments = 0;

  // parsing the command line arguments
  for (int i = 1; i < argc; i++) {
    char *equals_pointer = strchr(argv[i], '=');

    // check if the flag is a number and not index 1 or 2
    if (i == 1 && is_string_number(argv[i])) {
      if (i + 1 < argc && is_string_number(argv[i + 1])) {
        *samples = atoi(argv[i]);
        *tdelay = atoi(argv[i + 1]);
        propositional_arguments = 1;
        i++;
      } else {
        printf("Invalid set of arguments\n");
        return -1;
      }
    } else if (i != 1 && i != 2 && is_string_number(argv[i])) {
      printf("Invalid set of arguments\n");
      return -1;
    } else if (strcmp(argv[i], "--memory") == 0) {
      system_monitoring_flags[0] = 1;
    } else if (strcmp(argv[i], "--cpu") == 0) {
      system_monitoring_flags[1] = 1;
    } else if (strcmp(argv[i], "--cores") == 0) {
      system_monitoring_flags[2] = 1;
    } else {
      //handle the case if the curr argvp[i] might be --tdelay=N, --samples=N
      if (equals_pointer != NULL) {
        char *sub_string = strtok(argv[i], "=");
        char *val_str = strtok(NULL, "=");
        if (strcmp(sub_string, "--tdelay") == 0) {
          if (propositional_arguments == 1) {
            printf("Invalid set of arguments: cannot mix propositional "
                   "arguments and --tdelay\n");
  
            return -1;
          }
          if (!is_string_number(val_str)) {
            printf("--tdelay=N: N must be an integer\n");
            return -1;
          }
          int tdelay_val = atoi(val_str);
          if (tdelay_flag == 1 && tdelay_val != *tdelay) {
            printf("Invalid set of arguments: multiple --tdelay=N flags with "
                   "different values\n");
            return -1;
          }
          if (tdelay_flag == 0) {
            *tdelay = tdelay_val;
            tdelay_flag = 1;
          }
        } else if (strcmp(sub_string, "--samples") == 0) {
          if (propositional_arguments == 1) {
            printf("Invalid set of arguments: cannot mix propositional "
                   "arguments and --samples\n");
            return -1;
          }
          if (!is_string_number(val_str)) {
            printf("--samples=N: N must be an integer\n");
            return -1;
          }
          int samples_val = atoi(val_str);
          if (samples_flag == 1 && samples_val != *samples) {
            printf("Invalid set of arguments: multiple --samples=N flags with "
                   "different values\n");
            return -1;
          }
          if (samples_flag == 0) {
            *samples = samples_val;
            samples_flag = 1;
          }
        } else {
          printf("Unrecognized argument %s\n", argv[i]);
          return -1;
        }
      }
    }
  }

  if (samples_flag != tdelay_flag) {
    printf("Must define both sampels and tdelay flags\n");
    return -1;
  }

  // If no flags were set but we used positional arguments for samples/tdelay,
  // default all system_monitoring_flags to 1
  if (propositional_arguments == 1 && system_monitoring_flags[0] == 0 &&
      system_monitoring_flags[1] == 0 && system_monitoring_flags[2] == 0) {
    system_monitoring_flags[0] = 1;
    system_monitoring_flags[1] = 1;
    system_monitoring_flags[2] = 1;
  }

  // If no flags were set but we used flags arguments for samples/tdelay,
  // default all system_monitoring_flags to 1
  if (samples_flag == 1 && tdelay_flag == 1 &&
      system_monitoring_flags[0] == 0 && system_monitoring_flags[1] == 0 &&
      system_monitoring_flags[2] == 0) {
    system_monitoring_flags[0] = 1;
    system_monitoring_flags[1] = 1;
    system_monitoring_flags[2] = 1;
  }

  // If absolutely no args, default to everything
  if (argc == 1) {
    system_monitoring_flags[0] = 1;
    system_monitoring_flags[1] = 1;
    system_monitoring_flags[2] = 1;
  }

  return 0;
}





void system_monitoring_tool_indexes(int system_monitoring_flags[3],
                                    int indexes[3]) {
  ///_|> descry: this function sets a relative reference point to used to print out the each desired
  //graphs given the user flags 
  ///_|> int system_monitoring_flags[3]:int system_monitoring_flags[3]: indicates the arrayo which stores the integer flags of the graphs we want to output
  // in the program 0 not to show 1 to show for cpu, memeory,cores graphs used to figure out where to put
  // the reference points
  ///_|>  int indexes[3]: this indicates the array which will store the reference points for the graphs
  // so at index 0 its for memory graph at index 1 its for cpu graph and at index 2 its for num of cores.
  
                                      

  if (system_monitoring_flags[0] == 1 && system_monitoring_flags[1] == 1 &&
      system_monitoring_flags[2] == 1) {
    indexes[0] = 4;
    indexes[1] = 20;
    indexes[2] = 34;
  } else if (system_monitoring_flags[0] == 1 &&
             system_monitoring_flags[1] == 1 &&
             system_monitoring_flags[2] == 0) {
    indexes[0] = 4;
    indexes[1] = 20;
    indexes[2] = 0;
  } else if (system_monitoring_flags[0] == 0 &&
             system_monitoring_flags[1] == 1 &&
             system_monitoring_flags[2] == 1) {
    indexes[0] = 0;
    indexes[1] = 4;
    indexes[2] = 18;
  } else if (system_monitoring_flags[0] == 1 &&
             system_monitoring_flags[1] == 0 &&
             system_monitoring_flags[2] == 1) {
    indexes[0] = 4;
    indexes[1] = 0;
    indexes[2] = 20;
  } else if (system_monitoring_flags[0] == 0 &&
             system_monitoring_flags[1] == 0 &&
             system_monitoring_flags[2] == 1) {
    indexes[0] = 0;
    indexes[1] = 0;
    indexes[2] = 4;
  } else if (system_monitoring_flags[0] == 0 &&
             system_monitoring_flags[1] == 1 &&
             system_monitoring_flags[2] == 0) {
    indexes[0] = 0;
    indexes[1] = 4;
    indexes[2] = 0;
  } else if (system_monitoring_flags[0] == 1 &&
             system_monitoring_flags[1] == 0 &&
             system_monitoring_flags[2] == 0) {
    indexes[0] = 4;
    indexes[1] = 0;
    indexes[2] = 0;
  }
}

