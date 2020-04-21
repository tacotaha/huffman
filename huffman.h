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

typedef void (*huff_node_func)(huff_node_t *);

static inline huff_node_t *new_huff_node() {
  huff_node_t *n = malloc(sizeof(huff_node_t));
  if (!n) {
    perror("huff_node_t allocation failed!\n");
    exit(1);
  }
  return n;
}

static inline void print_huff_node(huff_node_t * node) {
  if (node)
    printf("char: %d -> freq: %ld", node->symb, node->freq);
}

// construct a frequency table
uint64_t *count_freqs(FILE *);

// build the huffman tree
huff_node_t *build_huff_tree(pqueue_t *);

// bottom-up traversal to generate code 
uint64_t get_huff_code(huff_node_t *, size_t *);

// apply func to nodes in order 
void inorder_traverse(huff_node_t *, huff_node_func);

#endif                          /* HUFFMAN_H */
