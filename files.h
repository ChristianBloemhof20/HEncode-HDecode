#include <stdio.h>

#ifndef FILES_H
#define FILES_H

int open_file(char *file_name);
/* Take an input file and open it with the "read()" command */

int create_output_file(int inputs, char *file_name[]);
/* Create an output file or print to stdout */

int get_input(int *inputs, char *file_name[]);
/* Check to see if input is a file or a dash, return the fd */

#endif