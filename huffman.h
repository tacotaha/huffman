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

static void print_bit_pattern(uint32_t bits, uint32_t len) {
  for (int i = len - 1; i >= 0; --i)
    printf("%d", (bits >> i) & 1);
}

// encode file contents
int encode(const char *, const char *);

// decode a file
int decode(const char *);

// construct a frequency table
uint64_t *count_freqs(FILE *);

// build the huffman tree
huff_node_t *build_huff_tree(pqueue_t *);

// bottom-up traversal to generate code 
uint64_t get_huff_code(huff_node_t *, uint32_t *);

// build the huffman code table
huff_code_t *build_huff_table(huff_node_t **);

// write out file header
void write_header(FILE *, huff_code_t *);

// parse the file header
huff_code_t *read_header(FILE *, uint32_t *);

#endif                          /* HUFFMAN_H */
