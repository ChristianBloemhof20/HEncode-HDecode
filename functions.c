#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <stdint.h>

#include "functions.h"
#include "files.h"

#define ARRAY_SIZE 255
#define SIZE 1024
#define BITS 8

    unsigned
    hash(char s)
{
    unsigned hashval;

    for (hashval = 0; s != '\0'; s++)
    {
        hashval = s + 31 * hashval;
    }

    return hashval % ARRAY_SIZE;
}

void input_chars(int fd,
                letter **char_array,
                int *char_inserted,
                int size)
{
    char ch;
    unsigned hashval;
    letter *node;

    while (read(fd, &ch, sizeof(uint8_t)))
    {
        hashval = hash(ch);
        node = malloc(sizeof(letter));
        node->letter = ch;
        node->freq = 1;
        node->next = NULL;
        node->left = NULL;
        node->right = NULL;
        if (place(char_array, node, &hashval, char_inserted))
        {
            *char_inserted = *char_inserted + 1;
        }
    }
}

int place(letter *char_array[],
          letter *node,
          unsigned *index,
          int *array_size)
{
    while (char_array[*index] != NULL)
    {
        if (char_array[*index]->letter == node->letter)
        {
            char_array[*index]->freq = char_array[*index]->freq + 1;
            free(node);
            return 0;
        }
        *index = (*index + 1) % *array_size;
    }
    char_array[*index] = node;

    return 1;
}

void remove_null_spaces(letter **char_array,
                        letter **final_array)
{
    int i;
    int index = 0;

    for (i = 0; i < ARRAY_SIZE; i++)
    {
        if (char_array[i] != NULL)
        {
            final_array[index] = char_array[i];
            index++;
        }
    }

    free(char_array);
}

int letter_comp(const void *p1, const void *p2)
{
    letter *char1 = *(letter **)p1;
    letter *char2 = *(letter **)p2;

    if (char1->freq > char2->freq)
    {
        return 1;
    }
    else if (char1->freq < char2->freq)
    {
        return -1;
    }
    else
    {
        if (char1->letter > char2->letter)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }
}

void print_array(letter *final_array[], int *char_inserted)
{
    int i;

    for (i = 0; i < *char_inserted; i++)
    {
        printf("%d %c\n", final_array[i]->freq, final_array[i]->letter);
    }
}

letter *create_linked_list(letter **final_array, 
                            int char_inserted, 
                            letter *head)
{
    letter *tail;
    letter *lp;

    int i;

    tail = head;
    for (i = 1; i < char_inserted; i++)
    {
        lp = final_array[i];
        tail->next = lp;
        tail = lp;
    }

    return tail;
}

void print_linked_list(letter *head)
{
    letter *lp;

    printf("\nHead\n");
    for (lp = head; lp != NULL; lp = lp->next)
    {
        printf("%d %c\n", lp->freq, lp->letter? lp->letter: '-');
    }
    printf("Tail\n\n");
}

letter *create_huffman_tree(letter *head, letter *tail)
{
    while (head->next != NULL)
    {
        head = huf_nodes(head, tail);
        if (tail->next != NULL)
        {
            tail = tail->next;
        }
    }
    return head;
}

letter *huf_nodes(letter *head, letter *tail)
{
    letter *freq_node = malloc(sizeof(letter));
    letter *lp;
    letter *prev_node;
    letter *first_node = head;
    letter *second_node = head->next;

    /* Create the frequency node */
    freq_node->freq = first_node->freq + second_node->freq;
    freq_node->left = first_node;
    freq_node->right = second_node;

    for (prev_node = first_node, lp = second_node;
        lp != NULL;
        prev_node = prev_node->next, lp = lp->next)
    {
        if (freq_node->freq <= lp->freq)
        {
            freq_node->next = lp;
            prev_node->next = freq_node;
            return second_node->next;
        }
    }

    /* Since we've made it all the way through the loop,
    * prev_node should be pointing at the tail */
    if (freq_node->freq > prev_node->freq)
    {
        prev_node->next = freq_node;
        if (second_node->next)
        {
            return second_node->next;
        }
    }

    return freq_node;
}

void print_ht(FILE *where, letter *node)
{
    print_ht_helper(where, node, 0);
}

void print_ht_helper(FILE *where, letter *node, int depth) {
    if (node) {
        print_ht_helper(where, node->right, depth+1);
        fprintf(where, "%*s%c (count: %d code: %s)\n",
                8*depth, "",
                node->letter,
                node->freq,
                ((node->bits? node->bits: "")));
        print_ht_helper(where, node->left, depth+1);
    }
}

void generate_encodings(letter *node,
                            char *bit,
                            char *bit_string,
                            int *string_size)
{
    char *final_string;
    /* We input '2' in the original function so we know it's the head */
    if (strcmp(bit, "2"))
    {
        *string_size = *string_size + 1;
        strncat(bit_string, bit, 1);
    }

    /* If we are at a leaf */
    if (node->left == NULL)
    {
        final_string = malloc(sizeof(char *) * *string_size);
        strcpy(final_string, bit_string);
        node->bits = final_string;
    }
    else
    {
        /* Go left */
        generate_encodings(node->left, "0", bit_string, string_size);
        /* Go right */
        generate_encodings(node->right, "1", bit_string, string_size);
    }
    bit_string[strlen(bit_string) - 1] = 0;
    string_size = string_size - 1;
}

void print_encodings(letter *array[], int *inserted)
{
    int i;

    for (i = 0; i < *inserted; i++)
    {
        printf("0x%2x: %s\n", array[i]->letter, array[i]->bits);
    }
}

void print_node(letter *node) {
    printf("Node: %d %c\n", node->freq, node->letter);
}

char *bit_converter(int num, char *bits) {
    int i;
    unsigned int mask;

    mask = 1 << (sizeof(uint64_t) - 1);
    for (i = 0; mask; i++) {
        if (num & mask) {
            bits[i] = '1';
        }
        else {
            bits[i] = '0';
        }
        mask>>=1;
    }
    bits[i] = '\0';

    return bits;
}

void write_header(letter *array[], int *char_inserted, int fd)
{
    int i;
    int freq;

    *char_inserted = *char_inserted - 1;
    write(fd, char_inserted, sizeof(uint8_t));

    for (i = 0; i <= *char_inserted; i++) {
        freq = htonl(array[i]->freq);
        write(fd, &array[i]->letter, sizeof(uint8_t));
        write(fd, &freq, sizeof(uint32_t));
    }
}


void encode(letter *final_array[],
            int char_inserted,
            int write_fd,
           char *file_name)
{
    int i, fd, num, byte = 8;
    unsigned int c = 0x0;
    char ch;
    char *bits;

    fd = open_file(file_name);

    /* Get the bitstring for the character pulled */
    while (read(fd, &ch, sizeof(uint8_t))) {
        for (i = 0; i < char_inserted + 1; i++) {
            if (final_array[i]->letter == ch) {
                bits = final_array[i]->bits;
            }
        }

        /* While we are not at the end of our encoding */
        while (bits[0] != '\0') {
            /* Pull the 1 or 0 then move the address pointer */
            num = ((int)bits[0] - '0');
            bits++;
            c = c | num;
            byte--;

            /* If we've written all our bites, write and start over */
            if (byte == 0) {
                write(write_fd, &c, sizeof(uint8_t));
                c = 0x0;
                byte = 8;
            }

            c <<= 1;
        }
    }

        /* If there are still values needed when we are done, pad with 0's */
        if (byte > 0)
    {
        while (byte != 1) {
            c <<= 1;
            c = c | 0;
            byte--;
        }
        write(write_fd, &c, sizeof(uint8_t));
    }
    close(fd);
}

void free_all(letter *node) {
    
    /* Check if at leaf */
    if (node->left == NULL) {
        free(node->bits);
        free(node);
    }
    /* Otherwise not at a leaf */
    else {
        free_all(node->left);
        free_all(node->right);
        free(node);
    }
}

int ascii_comp(const void *p1, const void *p2) {
    letter *char1 = *(letter **)p1;
    letter *char2 = *(letter **)p2;

    if (char1->letter < char2->letter) {
        return -1;
    }
    else {
        return 1;
    }
}

void parse_header(letter **char_array, int char_inserted, int fd)
{
    int i;
    char ch;
    int freq;
    letter *node;
    unsigned hashval;

    for (i = 0; i < char_inserted; i++) {
        /* Get the character */
        read(fd, &ch, sizeof(uint8_t));

        /* Get the frequency */
        read(fd, &freq, sizeof(uint32_t));
        freq = ntohl(freq);

        /* Hash and place in array */
        hashval = hash(ch);
        node = malloc(sizeof(letter));
        node->letter = ch;
        node->freq = freq;
        place(char_array, node, &hashval, &char_inserted);
    }
}

int get_freq(letter **array, int size) {
    int i;
    int freq = 0;

    for (i = 0; i < size; i++) {
        freq = freq + array[i]->freq;
    }

    return freq;
}

char *get_decoding_string(int fd) {
    char *string;
    char buffer[33];
    unsigned int ch;
    int size = 0;
    int i = 0;

    string = NULL;
    while (read(fd, &ch, sizeof(uint8_t))) {
        ch = ch & 0xFF;
        if (i > size - 2) {
            size += SIZE;
            string = realloc(string, size);
        }
            strncat(string, bit_converter(ch, buffer),
                    strlen(bit_converter(ch, buffer)));
        i += strlen(bit_converter(ch, buffer));
    }

    return string;
}

void decode(letter *root, 
            letter **array, 
            int fd, 
            int freq, 
            int char_inserted, 
            int argc, 
            char *argv[])
{
    int i;
    int index = 0;
    char *encodings;

    encodings = get_decoding_string(fd);

    for (i = 0; i < freq; i++) {
        traverse(root, encodings, &index, 1);
    }
}

void traverse(letter *node, char *encodings, int *index, int fd) {
    if (node->left == NULL) {
        write(fd, &node->letter, sizeof(uint8_t));
    }
    else if (encodings[*index] == '0') {
        *index = *index + 1;
        traverse(node->left, encodings, index, fd);
    }
    else {
        *index = *index + 1;
        traverse(node->right, encodings, index, fd);
    }
}
