#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "huffman.h"
#include "utils/pqueue.h"

uint64_t* count_freqs(FILE *fp){ 
    char c;
    uint64_t *freqs = malloc(256 * sizeof(uint64_t)); 

    if(freqs){
        memset(freqs, 0, 256 * sizeof(uint64_t));
        while((c = fgetc(fp)) != EOF)
            ++freqs[(int)c];
    }
    
    return freqs; 
}
