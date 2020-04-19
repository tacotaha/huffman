#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pqueue.h"
#define SIZE 10

void print_int(void *data){
    if(data)
        printf("%d", *(int*)data);
}

int main(int argc, char* argv[]){
  srand(time(0));

  pqueue_t *pq = pqueue_init();

  printf("Populating the queue with numbers 0 to %d...\n", SIZE);
  
  for(int i = 0; i < SIZE; i++){
      int *tmp = malloc(sizeof(int));
      *tmp = i;
      pqueue_insert(pq, rand() % 100, tmp);

  }

  printf("Current State of the Priority Queue:\n");

  pqueue_print(pq, print_int);

  printf("Current front: ");
  print_int(pqueue_front(pq));
  printf("\n");
   
  while(!pqueue_empty(pq)){
    printf("Removing ");
    print_int(pqueue_front(pq));
    printf("...\n");
    pqueue_pop(pq);
  }
  pqueue_destroy(&pq);
  
  return 0;
}
