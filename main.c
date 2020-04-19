#include <stdio.h>
#include "huffman.h"

int main(int argc, char** argv){    

    FILE *fp = fopen(argv[1], "rb"); 
    if(!fp){
        fprintf(stderr, "Failed to open file!\n");
        return 2;
    }

    uint64_t *freqs = count_freqs(fp);

    for(int i = 0; i < 256; ++i)
        if(freqs[i])
            printf("%d -> %ld\n", i, freqs[i]);

    
    return 0;
}
