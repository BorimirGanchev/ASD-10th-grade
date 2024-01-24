#ifndef _HEAP_H_
#define _HEAP_H_

typedef struct vector_t vector_t;
void swap(int *a, int *b);
void siftUp(vector_t *v, int i);
void siftDown(vector_t *v, int i);
void heapify(vector_t *v);
#endif