#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"
#include "utils/pqueue.h"


void print_huff_node(huff_node_t* node){
      if(node)
        printf("char: %d -> freq: %ld\n", node->symb, node->freq);
}

int main(int argc, char **argv) {
  FILE *fp = fopen("../huckfinn.txt", "rb");
  if (!fp) {
    fprintf(stderr, "Failed to open file!\n");
    return 2;
  }
  
  // Build the frequency table
  uint64_t *freqs = count_freqs(fp);
 
  // Build the huffman tree
  huff_node_t *root = build_huff_tree(freqs);

  inorder_traverse(root, print_huff_node);

  free(freqs);
  fclose(fp);
  return 0;
}
