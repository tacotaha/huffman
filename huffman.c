#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "huffman.h"
#include "utils/pqueue.h"

uint64_t *count_freqs(FILE * fp) {
  char c;
  uint64_t *freqs = malloc(NUM_SYMBS * sizeof(uint64_t));

  if (freqs) {
    memset(freqs, 0, NUM_SYMBS * sizeof(uint64_t));
    while ((c = fgetc(fp)) != EOF)
      ++freqs[(int) c];
  }

  return freqs;
}

huff_node_t *build_huff_tree(pqueue_t * pq) {
  huff_node_t *left, *right, *new = NULL;

  if (pq)
    while (pq->size > 1) {
      left = pqueue_front(pq);
      pqueue_pop(pq);

      right = pqueue_front(pq);
      pqueue_pop(pq);

      new = new_huff_node();
      new->freq = left->freq + right->freq;
      new->symb = INTERNAL_NODE;
      new->left = left;
      new->right = right;
      new->parent = NULL;

      left->parent = right->parent = new;
      pqueue_insert(pq, new->freq, new);
    }

  return new;
}

uint64_t get_huff_code(huff_node_t * leaf, size_t *len) {
  uint64_t code = 0;
  size_t num_bits = 0;
  huff_node_t *child = NULL;

  // bottom-up traversal
  while (leaf) {
    child = leaf;
    leaf = leaf->parent;
    if (leaf)
      code |= ((child == leaf->right) << num_bits++);
  }

  if (len)
    *len = num_bits;
  return code;
}

void inorder_traverse(huff_node_t * root, huff_node_func fn) {
  if (root) {
    inorder_traverse(root->left, fn);
    if (root->symb != INTERNAL_NODE)
      fn(root);
    inorder_traverse(root->right, fn);
  }
}
