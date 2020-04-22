#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "huffman.h"
#include "utils/pqueue.h"
#include "utils/bitio.h"

int encode(const char *in, const char *out) {
  int c;
  uint8_t sf;
  huff_code_t code;
  bitio_t *b;
  FILE *fout, *fp = fopen(in, "rb");
  pqueue_t *pq = pqueue_init();
  huff_node_t *n, *leaves[NUM_SYMBS];

  if (!fp) {
    fprintf(stderr, "Failed to open file!\n");
    return -1;
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

  // build the huffman tree
  build_huff_tree(pq);

  rewind(fp);

  huff_code_t *table = build_huff_table(leaves);

  // dump the huffman table 
  fout = fopen(out, "wb");
  write_header(fout, table);
  fclose(fout);

  // append the encoded data 
  b = bopen(out, APPEND);
  while ((c = fgetc(fp)) != EOF) {
    code = table[c];
    for (int i = 0; i < code.len; ++i) {
      sf = code.len - i - 1;
      bwrite(b, (code.code & (1 << sf)) >> sf);
    }
  }
  bclose(b);

  free(freqs);
  fclose(fp);
  return 0;
}

int decode(const char *file) {
  uint32_t tab_len;
  huff_code_t *table = NULL;
  FILE *fp = fopen(file, "rb");
  if (fp) {
    table = read_header(fp, &tab_len);
    fclose(fp);
  }
  return 0;
}

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

huff_code_t *build_huff_table(huff_node_t ** leaves) {
  huff_code_t *table = malloc(sizeof(huff_code_t) * NUM_SYMBS);
  if (table)
    for (int i = 0; i < NUM_SYMBS; ++i)
      table[i].code = get_huff_code(leaves[i], &table[i].len);
  return table;
}

uint64_t get_huff_code(huff_node_t * leaf, uint32_t * len) {
  uint64_t code = 0;
  uint32_t num_bits = 0;
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

void write_header(FILE * fp, huff_code_t * table) {
  /*
   * header format:
   * <num symbols (2 bytes)>
   * <length (4 bytes)><code (length bits)>
   */

  uint16_t uniq_symbs = 0;
  for (int i = 0; i < NUM_SYMBS; ++i)
    uniq_symbs += (table[i].len > 0);

  fwrite(&uniq_symbs, 1, sizeof(uint16_t), fp);

  for (uint16_t i = 0; i < NUM_SYMBS; ++i)
    if (table[i].len) {
      fwrite(&table[i].len, 1, sizeof(uint32_t), fp);
      fwrite(&table[i].code, 1, sizeof(uint64_t), fp);
    }
}

huff_code_t *read_header(FILE * fp, uint32_t * len) {
  uint16_t uniq_symbs = 0;
  fread(&uniq_symbs, 1, sizeof(uint16_t), fp);

  huff_code_t *table = malloc(sizeof(huff_code_t) * uniq_symbs);
  if (table) {
    for (int i = 0; i < uniq_symbs; ++i) {
      fread(&table[i].len, 1, sizeof(uint32_t), fp);
      fread(&table[i].code, 1, sizeof(uint64_t), fp);
    }
  }
  if (len)
    *len = uniq_symbs;
  return table;
}

void inorder_traverse(huff_node_t * root, huff_node_func fn) {
  if (root) {
    inorder_traverse(root->left, fn);
    if (root->symb != INTERNAL_NODE)
      fn(root);
    inorder_traverse(root->right, fn);
  }
}
