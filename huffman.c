#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "huffman.h"
#include "utils/pqueue.h"

uint64_t *count_freqs(FILE * fp) {
  char c;
  uint64_t *freqs = malloc(256 * sizeof(uint64_t));

  if (freqs) {
    memset(freqs, 0, 256 * sizeof(uint64_t));
    while ((c = fgetc(fp)) != EOF)
      ++freqs[(int) c];
  }

  return freqs;
}

huff_node_t *build_huff_tree(uint64_t * freqs) {
  huff_node_t *left, *right, *new;
  pqueue_t *pq = pqueue_init();

  // enqueue all leaf nodes 
  for (int i = 0; i < 256; ++i) {
    huff_node_t *n = new_huff_node();
    n->symb = i;
    n->freq = freqs[i];
    n->left = n->right = NULL;
    pqueue_insert(pq, i, n);
  }

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

    pqueue_insert(pq, new->freq, new);
  }

  return new;
}

void inorder_traverse(huff_node_t * root, huff_node_func fn) {
  if (root) {
    inorder_traverse(root->left, fn);
    if (root->symb != INTERNAL_NODE)
      fn(root);
    inorder_traverse(root->right, fn);
  }
}
