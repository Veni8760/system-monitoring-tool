
/*
 *  Header file for our helper functions used in my SMT.
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


#ifndef __helper_func_header
#define __helper_func_header

void terminal_cursor_mover(int row, int col);
  ///_|> descry: this function moves the terminal cursor to the given row and column in the termial
  ///_|> int row: this indicates the row in the terminal to move the cursor 
  ///_|> int col: this indicates the col in the terminal to move the cursor 
  ///_|>  returns: the function returns an int of the number of cores of the cpu
  
  
int is_string_number(char *str);
  ///_|> descry: this function checks if the given strig is a integer number
  ///_|> char *str: this indicates the string we want to parse to check if its a number
  ///_|>  returns: the function returns int 1 if the characters are all digits thus a number
  // otherwise it returns 0


double kb_to_gb(long kilobytes);
///_|> descry: this function converts a given kilobytes into gigabytes
  ///_|> long kilobytes: this indicates the kilobytes we want to convert
  ///_|>  returns: the function returns double of the the kilobytes in gigabytes
  
double kHz_to_GHz(double frequency_in_kHz);
 ///_|> descry: this function converts a given  kilohertz to gigahertz.
  ///_|> double frequency_in_kHz: this indicates the kilohertz  we want to convert
  ///_|>  returns: the function returns double of the the  kilohertz in gigahertz

#endif