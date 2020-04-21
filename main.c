#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"
#include "utils/pqueue.h"

void print_bit_pattern(uint32_t bits, uint32_t len) {
  for (int i = len - 1; i >= 0; --i)
    printf("%d", (bits >> i) & 1);
}

int main(int argc, char **argv) {
  uint64_t code;
  size_t num_bits;

  FILE *fp = fopen("../huckfinn.txt", "rb");
  pqueue_t *pq = pqueue_init();
  huff_node_t *n, *root, *leaves[NUM_SYMBS];

  if (!fp) {
    fprintf(stderr, "Failed to open file!\n");
    return 2;
  }
  // build the frequency table
  uint64_t *freqs = count_freqs(fp);

  // enqueue all leaf nodes 
  for (int i = 0; i < NUM_SYMBS; ++i) {
    if (!freqs[i])
      n = NULL;
    else {
      n = new_huff_node();
      n->symb = i;
      n->freq = freqs[i];
      n->left = n->right = NULL;
      pqueue_insert(pq, n->freq, n);
    }
    leaves[i] = n;
  }

  // Build the huffman tree
  root = build_huff_tree(pq);

  fp = fopen("../huckfinn.txt", "rb");
  int c;

  while ((c = fgetc(fp)) != EOF) {
    code = get_huff_code(leaves[c], &num_bits);
    print_bit_pattern(code, num_bits);
  }
  printf("\n");

  free(freqs);
  fclose(fp);
  return 0;
}
