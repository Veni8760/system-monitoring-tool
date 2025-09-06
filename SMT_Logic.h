/*
 *  Header file for our functions that handle the logic of the System Monitoring Tool.
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

 
#ifndef __SMT_Logic_header
#define __SMT_Logic_header


int parse_command_line_arguments(int argc, char **argv, int *samples,
                                 int *tdelay, int system_monitoring_flags[3]);
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



void system_monitoring_tool_indexes(int system_monitoring_flags[3],
                                    int indexes[3]);
    ///_|> descry: this function sets a relative reference point to used to print out the each desired
  //graphs given the user flags 
  ///_|> int system_monitoring_flags[3]:int system_monitoring_flags[3]: indicates the arrayo which stores the integer flags of the graphs we want to output
  // in the program 0 not to show 1 to show for cpu, memeory,cores graphs used to figure out where to put
  // the reference points
  ///_|>  int indexes[3]: this indicates the array which will store the reference points for the graphs
  // so at index 0 its for memory graph at index 1 its for cpu graph and at index 2 its for num of cores.
  
     

// void generate_system_monitoring_tool(int argc, char **argv);
// ///_|> descry: this function handles all logic to run the system_monitoring_tool
//   //concurrently
//   ///_|> int argc:  indicates the  number of CLA 
//   ///_|> char **argv: indicates the  array of CLA string
//   ///_|>  returns: the function returns nothing.




#endif