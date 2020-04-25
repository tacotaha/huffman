#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdint.h>
#include "utils/pqueue.h"

#define NUM_SYMBS 256
#define INTERNAL_NODE (NUM_SYMBS + 1)

typedef struct huff_node huff_node_t;
struct huff_node {
  uint16_t symb;
  uint64_t freq;
  huff_node_t *parent, *left, *right;
};

typedef struct huff_code {
  uint64_t code;
  uint32_t len;
} huff_code_t;

typedef void (*huff_node_func)(huff_node_t *);

static inline huff_node_t *new_huff_node() {
  huff_node_t *n = malloc(sizeof(huff_node_t));
  if (!n) {
    perror("huff_node_t allocation failed!\n");
    exit(1);
  }
  return n;
}

// encode file contents
size_t encode(const char *, const char *);

// decode a file
size_t decode(const char *, const char *);

// construct a frequency table
uint64_t *count_freqs(FILE *);

// build the huffman tree
huff_node_t *build_huff_tree(uint64_t *, huff_node_t ***);

// free the contents of the tree 
void destroy_huff_tree(huff_node_t *);

// bottom-up traversal to generate code 
uint64_t get_huff_code(huff_node_t *, uint32_t *);

// build the huffman code table
huff_code_t *build_huff_table(huff_node_t **);

// dump frequency table as file header 
void write_header(FILE *, uint64_t *);

// parse the file header & return frequency table
uint64_t *read_header(FILE *);

#endif                          /* HUFFMAN_H */
