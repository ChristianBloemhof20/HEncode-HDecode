#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#include "functions.h"
#include "files.h"

#define ARRAY_SIZE 255

int main (int argc, char *argv[]) {
    letter **char_array;
    letter **final_array;
    letter *head;
    letter *tail;
    letter *root;
    int fd;
    int char_inserted;
    int string_size = 0;
    int freq;
    char bit_string[ARRAY_SIZE];

    fd = get_input(&argc, argv);

    /* Get Char Inserted */
    read(fd, &char_inserted, sizeof(uint8_t));
    char_inserted = char_inserted + 1;

    /* Create an array with the proper size of all the characters */
    char_array = malloc(sizeof(letter *) * ARRAY_SIZE);
    parse_header(char_array, char_inserted, fd);
    final_array = malloc(sizeof(letter *) * char_inserted);
    remove_null_spaces(char_array, final_array);
    qsort(final_array, char_inserted, sizeof(letter *), letter_comp);
    freq = get_freq(final_array, char_inserted);

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

    decode(root, final_array, fd, freq, char_inserted, argc, argv);
    return 0;
}