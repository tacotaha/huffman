#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdint.h>

#define INTERNAL_NODE 257

typedef struct huff_node huff_node_t;
struct huff_node {
  uint16_t symb;
  uint64_t freq;
  huff_node_t *left, *right;
};

typedef void (*huff_node_func)(huff_node_t *);

// Construct a frequency table
uint64_t *count_freqs(FILE *);

// Build the huffman tree
huff_node_t *build_huff_tree(uint64_t *);

// Apply func to nodes in order 
void inorder_traverse(huff_node_t *, huff_node_func);

static inline huff_node_t *new_huff_node() {
  huff_node_t *n = malloc(sizeof(huff_node_t));
  if (!n) {
    perror("huff_node_t allocation failed!\n");
    exit(1);
  }
  return n;
}

#endif                          /* HUFFMAN_H */
