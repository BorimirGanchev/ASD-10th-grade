#include <stdio.h>
#include <stdlib.h>

#define MAX_HEAP_SIZE 100

struct MinHeap {
    int *array;
    int capacity;
    int size;
};

struct MinHeap* createMinHeap(int capacity) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->capacity = capacity;
    minHeap->size = 0;
    minHeap->array = (int*)malloc(capacity * sizeof(int));
    return minHeap;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(struct MinHeap* minHeap, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < minHeap->size && minHeap->array[left] < minHeap->array[smallest])
        smallest = left;

    if (right < minHeap->size && minHeap->array[right] < minHeap->array[smallest])
        smallest = right;

    if (smallest != i) {
        swap(&minHeap->array[i], &minHeap->array[smallest]);
        minHeapify(minHeap, smallest);
    }
}

void insertKey(struct MinHeap* minHeap, int key) {
    if (minHeap->size == minHeap->capacity) {
        printf("Err\n");
        return;
    }

    int i = minHeap->size;
    minHeap->array[i] = key;
    minHeap->size++;
    while (i != 0 && minHeap->array[(i - 1) / 2] > minHeap->array[i]) {
        swap(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int delMin(struct MinHeap* minHeap) {
    if (minHeap->size <= 0)
        return minHeap->size;

    if (minHeap->size == 1) {
        minHeap->size--;
        return minHeap->array[0];
    }

    int root = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    minHeap->size--;
    minHeapify(minHeap, 0);

    return root;
}

int main() {
    struct MinHeap* minHeap = createMinHeap(MAX_HEAP_SIZE);

    insertKey(minHeap, 3);
    insertKey(minHeap, 2);
    insertKey(minHeap, 15);
    insertKey(minHeap, 5);
    insertKey(minHeap, 4);
    insertKey(minHeap, 45);

    printf("Heap: ");
    for (int i = 0; i < minHeap->size; i++)
        printf("%d ", minHeap->array[i]);

    printf("\nRemove: %d\n", delMin(minHeap));
    for (int i = 0; i < minHeap->size; i++)
        printf("%d ", minHeap->array[i]);

    free(minHeap->array);
    free(minHeap);

    return 0;
}
