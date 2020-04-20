#include <stdio.h>
#include <stdlib.h>
#include "pqueue.h"

pqueue_t *pqueue_init() {
  pqueue_t *pq = malloc(sizeof(pqueue_t));
  if (pq != NULL) {
    pq->capacity = INITIAL_CAPACITY;
    pq->size = 0;
    pq->data = malloc(sizeof(Node *) * pq->capacity);
    if (pq->data == NULL) {
      free(pq);
      pq = NULL;
    }
  }
  return pq;
}

int pqueue_insert(pqueue_t * pq, int priority, void *data) {
  Node *node = (Node *) malloc(sizeof(Node));
  if (node == NULL)
    return -1;

  node->priority = priority;
  node->data = data;

  // too full...resize
  if (pq->size >= pq->capacity) {
    Node **new_data = malloc(sizeof(Node *) * pq->capacity * 2);

    if (!new_data)
      return -2;

    for (int i = 0; i < pq->size; i++)
      new_data[i] = pq->data[i];

    free(pq->data);

    pq->data = new_data;
    pq->capacity *= 2;
  }
  //Append the element to the end
  pq->data[pq->size++] = node;

  //Make sure the heap property holds
  heapify(pq);

  return 0;
}

void pqueue_pop(pqueue_t * pq) {
  if (pq->size) {
    // swap first & last elements
    Node *temp = pq->data[pq->size - 1];
    pq->data[pq->size - 1] = pq->data[0];
    pq->data[0] = temp;

    // "remove" the last element
    pq->size--;

    // Make sure the heap property hods
    heapify(pq);
  }
}

void *pqueue_front(const pqueue_t * pq) {
  return pq->size ? pq->data[0]->data : NULL;
}

int pqueue_empty(const pqueue_t * pq) {
  return (pq->size == 0);
}

void pqueue_destroy(pqueue_t ** pq) {
  for (int i = 0; i < (*pq)->capacity; i++)
    free((*pq)->data[i]);

  free((*pq)->data);
  free(*pq);
  *pq = NULL;
}

void pqueue_print(const pqueue_t * pq, print_func_t fn) {
  for (int i = 0; i < pq->size; i++) {
    printf("PRIORITY: %d", pq->data[i]->priority);
    printf(" Data: ");
    fn(pq->data[i]->data);
    printf("\n");
  }
}

void fix_up(pqueue_t * pq, int index) {
  int index_of_parent;
  Node *temp;

  if (index > 0) {
    index_of_parent = (index - 1) / 2;

    if (pq->data[index]->priority < pq->data[index_of_parent]->priority) {
      temp = pq->data[index];
      pq->data[index] = pq->data[index_of_parent];
      pq->data[index_of_parent] = temp;
      fix_up(pq, index_of_parent);
    }
  }
}

void fix_down(pqueue_t * pq, int index) {
  int index_of_left_child;
  int index_of_right_child;
  int index_of_largest_child = pq->size;
  Node *temp;

  index_of_left_child = 2 * index + 1;
  index_of_right_child = 2 * index + 2;

  //two children
  if (index_of_right_child < pq->size)
    index_of_largest_child =
      (pq->data[index_of_left_child]->priority <
       pq->data[index_of_right_child]->priority)
      ? index_of_left_child : index_of_right_child;

  //only one child
  else if (index_of_left_child < pq->size)
    index_of_largest_child = index_of_left_child;

  if (index_of_largest_child < pq->size && pq->data[index]->priority
      > pq->data[index_of_largest_child]->priority) {
    temp = pq->data[index];
    pq->data[index] = pq->data[index_of_largest_child];
    pq->data[index_of_largest_child] = temp;
    fix_down(pq, index_of_largest_child);
  }

}

void heapify(pqueue_t * pq) {
  for (int i = (pq->size - 2) / 2; i >= 0; i--)
    fix_down(pq, i);
}
