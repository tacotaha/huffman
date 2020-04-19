/* Implements a priority queue using a minheap */

#ifndef PQUEUE_H
#define PQUEUE_H
 
#define INITIAL_CAPACITY 8

typedef struct Node{
  int priority;
  void *data;
}Node;

typedef struct pqueue_t{
  int size;
  int capacity;
  Node** data;
}pqueue_t;

typedef void (*print_func_t)(void*);

/* Priority Queue API */
pqueue_t* pqueue_init(void);
int pqueue_insert(pqueue_t*, int priority, void *data);
void pqueue_pop(pqueue_t*);
void* pqueue_front(const pqueue_t*);
int pqueue_empty(const pqueue_t*);
void pqueue_destroy(pqueue_t **);
void pqueue_print(const pqueue_t*, print_func_t);


/* Internal functions for maintaining the heap property */
void fix_up(pqueue_t*, int index);
void fix_down(pqueue_t*, int index);
void heapify(pqueue_t*);


#endif
