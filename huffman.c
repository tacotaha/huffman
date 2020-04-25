#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "huffman.h"
#include "utils/pqueue.h"
#include "utils/bitio.h"
#include <assert.h>

size_t encode(const char *in, const char *out) {
  int c;
  bitio_t *b;
  huff_code_t code;
  huff_node_t **leaves;
  size_t bytes_encoded = 0;
  FILE *fout, *fp = fopen(in, "rb");

  if (!fp) {
    fprintf(stderr, "Failed to open file!\n");
    return -1;
  }
  // build the frequency table
  uint64_t *freqs = count_freqs(fp);

  // build the huffman tree
  build_huff_tree(freqs, &leaves);

  rewind(fp);

  huff_code_t *table = build_huff_table(leaves);

  // dump the frequency table 
  fout = fopen(out, "wb");
  write_header(fout, freqs);

  // append the encoded data 
  b = bopen_f(fout, WRITE);
  while ((c = fgetc(fp)) != EOF) {
    assert(c < NUM_SYMBS);
    code = table[c];
    for (int i = 0; i < code.len; ++i)
      bwrite(b, (code.code >> (code.len - i - 1)) & 1);
    ++bytes_encoded;
  }

  bclose(b);
  free(freqs);
  fclose(fp);
  return bytes_encoded;
}

size_t decode(const char *in, const char *out) {
  char bit;
  bitio_t *b;
  uint64_t *freqs;
  FILE *fp_in, *fp_out;
  size_t bytes_decoded = 0;
  huff_node_t *root, *node, **leaves;

  fp_in = fopen(in, "rb");
  if (!fp_in)
    return -1;

  fp_out = fopen(out, "w");
  if (!fp_out) {
    fclose(fp_in);
    return -2;
  }

  freqs = read_header(fp_in);
  root = build_huff_tree(freqs, &leaves);

  b = bopen_f(fp_in, READ);

  node = root;
  while ((bit = bread(b)) != BEOF) {
    node = bit ? node->right : node->left;

    // reached a symbol
    if (node->left == NULL && node->right == NULL) {
      fwrite(&node->symb, 1, 1, fp_out);
      node = root;
      ++bytes_decoded;
    }
  }

  return bytes_decoded;
}

uint64_t *count_freqs(FILE * fp) {
  int c;
  uint64_t *freqs = malloc(NUM_SYMBS * sizeof(uint64_t));

  if (freqs) {
    memset(freqs, 0, NUM_SYMBS * sizeof(uint64_t));
    while ((c = fgetc(fp)) != EOF)
      ++freqs[c];
  }

  return freqs;
}

huff_node_t *build_huff_tree(uint64_t * freqs, huff_node_t *** leaves) {
  huff_node_t **leafs, *n, *left, *right, *new;

  pqueue_t *pq = pqueue_init();
  if (!pq)
    return NULL;

  leafs = malloc(sizeof(huff_node_t) * NUM_SYMBS);
  if (!leaves) {
    pqueue_destroy(&pq);
    return NULL;
  }
  // enqueue leaf nodes 
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
    leafs[i] = n;
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
    new->parent = NULL;

    left->parent = right->parent = new;
    pqueue_insert(pq, new->freq, new);
  }

  if (leaves)
    *leaves = leafs;
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

void write_header(FILE * fp, uint64_t * freqs) {
  /*
   * header format:
   * <num symbols (2 bytes)>
   * <symbol (2 bytes)><frequency (8 bytes)>
   */

  uint16_t uniq_symbs = 0;
  for (int i = 0; i < NUM_SYMBS; ++i)
    uniq_symbs += (freqs[i] != 0);

  fwrite(&uniq_symbs, 1, sizeof(uint16_t), fp);

  for (uint16_t i = 0; i < NUM_SYMBS; ++i)
    if (freqs[i]) {
      fwrite(&i, 1, sizeof(uint16_t), fp);
      fwrite(freqs + i, 1, sizeof(uint64_t), fp);
    }
}

uint64_t *read_header(FILE * fp) {
  uint16_t uniq_symbs, symb;
  fread(&uniq_symbs, 1, sizeof(uint16_t), fp);

  uint64_t *freqs = malloc(sizeof(uint64_t) * NUM_SYMBS);
  if (freqs) {
    memset(freqs, 0, sizeof(uint64_t) * NUM_SYMBS);
    for (int i = 0; i < uniq_symbs; ++i) {
      fread(&symb, 1, sizeof(uint16_t), fp);
      fread(freqs + symb, 1, sizeof(uint64_t), fp);
    }
  }
  return freqs;
}
