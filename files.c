#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "files.h"

int open_file(char *file_name) {
    int fd;

    if ((fd = open(file_name, O_RDONLY)) == -1) {
        fprintf(stderr, "%s: No such file or directory\n", file_name);
        exit(EXIT_FAILURE);
    }

    return fd;
}

int create_output_file(int inputs, char *file_name[]) {
    int fd;

    if (inputs < 3) {
        /* Setting fd to 1 has it set to stdout */
        fd = 1;
    }
    else {
        if ((fd = 
        open(file_name[2], 
            O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) == -1) {
            fprintf(stderr, "There was error opening file %s\n", file_name[2]);
            exit(EXIT_FAILURE);
        }
    }

    return fd;
}

int get_input(int *inputs, char *file_name[]) {
    int fd;

    if (*inputs == 1 || strcmp(file_name[1], "-") == 0) {
        /* Setting fd to 0 has it set to stdin */
        fd = 0;
    }
    else {
        if ((fd = open(file_name[1], O_RDONLY)) == -1) {
            fprintf(stderr, "%s: No such file or directory\n", file_name[1]);
            exit(EXIT_FAILURE);
        }
    }

    return fd;
}