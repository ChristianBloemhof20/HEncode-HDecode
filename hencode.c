#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "functions.h"
#include "files.h"

#define ARRAY_SIZE 255

int main(int argc, char *argv[]) {
    letter **char_array;
    letter **final_array;
    letter *root;
    letter *head;
    letter *tail;
    char bit_string[ARRAY_SIZE];
    int fd;
    int output_fd;
    int i;
    int char_inserted = 0;
    int string_size = 0;

    /* Opening files here */
    if (argc == 1 || argc > 3) {
        fprintf(stderr, 
                "usage //home/pn-cs357/demos/hencode infile [ outfile ]\n");
        exit(EXIT_FAILURE);
    }

    fd = open_file(argv[1]);
    output_fd = create_output_file(argc, argv);

    /* Create an array to take in all input chars */
    char_array = malloc(sizeof(letter) * ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        char_array[i] = NULL;
    }

    input_chars(fd, char_array, &char_inserted, ARRAY_SIZE);
    final_array = malloc(sizeof(letter) * char_inserted);
    remove_null_spaces(char_array, final_array);
    qsort(final_array, char_inserted, sizeof(letter *), letter_comp);

    if (close(fd) == -1) {
        perror("Error closing file descriptor\n");
        exit(EXIT_FAILURE);
    }

    /* We now should have our sorted array and can begin the huffman tree */
    head = malloc(sizeof(letter));
    head = final_array[0];
    tail = create_linked_list(final_array, char_inserted, head);
    root = create_huffman_tree(head, tail);
    generate_encodings(root, "2", bit_string, &string_size);

    /* We now have our encodings, we need to place them into a table and
    * transer our input file to our new output */
    /* We now need to sort the array based on ascii value */
    qsort(final_array, char_inserted, sizeof(letter *), ascii_comp);

    write_header(final_array, &char_inserted, output_fd);
    encode(final_array, char_inserted, output_fd, argv[1]);

    /* Close the files */

    free_all(root);
    free(final_array);

    close(output_fd);

    return 0;
}