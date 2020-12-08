#ifndef HENCODE_FUNCTIONS_H
#define HENCODE_FUNCTIONS_H

typedef struct letter letter;

struct letter{
    char letter;
    int freq;
    char *bits;
    letter *next;
    letter *left;
    letter *right;
};

unsigned hash(char s);
/* Hashing function to find where to place the letter in array */

void input_chars(int fd,
                    letter **char_array,
                    int *char_inserted,
                    int size);
/* Take in every single ascii character and place them
* into the array.  If there are repeats, increment the
* frequency */

void header_chars(int fd,
                    letter **char_array,
                    int *char_inserted);

int place(letter *char_array[],
            letter *node,
            unsigned *index,
            int *array_size);

void remove_null_spaces(letter **char_array, letter **final_array);
/* Place every value from the array into a new array of
* perfect size so that there are no blank spaces */

int letter_comp(const void *p1, const void *p2);
/* A function for qsort.  Will compare based on frequency
* and then based on ascii value if they're even */

void print_array(letter *final_array[], int *char_inserted);
/* A function to print out our array for testing */

letter *create_linked_list(letter **final_array, 
                            int char_inserted, 
                            letter *head);
/* Transfter the contents of the array to a linked list.
* returns the head. */

void print_linked_list(letter *head);
/* A function to print our linked list for testing */

letter *create_huffman_tree(letter *head, letter *tail);
/* Turns that linked list into a huffman tree.  Returns the
* new head of the tree */

letter *huf_nodes(letter *head, letter *tail);

void print_ht(FILE *where, letter *node);
/* A function to print our huffman tree for testing */

void print_ht_helper(FILE *where, letter *node, int depth);

void generate_encodings(letter *head,
                        char *bit,
                        char *bit_string,
                        int *string_size);
/* Traverse through the tree and find the bit-encoding for each
* letter.  This encoding will be placed into the node of the letter.
* These nodes will be placed into an array so that we can parse
* the array to find the codes. */

void print_encodings(letter *array[], int *inserted);
/* A function to print hex letters and encodings for testing */

void print_node(letter *node);
/* A function to print individual nodes */

char *bit_converter(int num, char *bits);

void write_header(letter *array[], int *char_inserted, int fd);
/* This function will write all the header values to the output file */

void encode(letter *final_array[], 
            int char_inserted, 
            int write_fd, 
            char *file_name);
/* Now that we have our table with our encodings, we want to parse 
* through the file again.  For each character we find we will parse
* throught he array until we find our character and place the bit encoding
* into the output file */

char *encoding_string(letter *final_array[], int char_inserted, int fd);

void free_all(letter *node);

int ascii_comp(const void *p1, const void *p2);

void parse_header(letter **char_array, int char_inserted, int fd);
/* This function goes through the header, grabbing every letterr and frequency
* and placing it into the array */

int get_freq(letter **array, int size);
/* Gets the total frequency of each character in the file */

char *get_decoding_string();
/* Grab 1 byte, convert it to a binary string, and append it 
* to our decoding string */

void decode(letter *root, 
            letter **array, 
            int fd, 
            int freq, 
            int char_inserted, 
            int argc, 
            char *argv[]);

void traverse(letter *node, char *encodings, int *index, int write_fd);

#endif