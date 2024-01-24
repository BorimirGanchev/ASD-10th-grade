#include <stdio.h>
#include "vector.h"

typedef struct {
    int* arr;
    int size;
} Vector;

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(Vector* v) {
    for (int i = v->size / 2 - 1; i >= 0; i--) {
        siftDown(v, i, v->size);
    }
}

void siftDown(Vector* v, int i, int size) {
    int swapped = 1;

    while (swapped) {
        swapped = 0;
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < size && v->arr[left] > v->arr[largest]) {
            largest = left;
        }
        if (right < size && v->arr[right] > v->arr[largest]) {
            largest = right;
        }

        if (i != largest) {
            swap(&v->arr[i], &v->arr[largest]);
            i = largest;
            swapped = 1;
        }
    }
}

void heapsort(Vector* v) {
    heapify(v);

    for (int i = v->size - 1; i > 0; i--) {
        swap(&v->arr[0], &v->arr[i]);
        siftDown(v, 0, i);
    }
}

int main() {
    Vector v;
    int arr[] = {20, 50, 15, 8, 26, 10, 16, 30};
    int n = sizeof(arr) / sizeof(arr[0]);

    v.arr = arr;
    v.size = n;

    printf("Original array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    heapsort(&v);

    printf("Sorted array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}

/*heapify функция:
Отнема O(n) време, защото единствено само коренът има възможност да е O(log n), а другите node-ове започеат от поне минимум -1 ниво, тоест половината възли са листа и няма накъде повече да се спускат, така че умножаваме по 0. Една четвърт са само едно ниво нагоре, една осма две и т.н. Само един възел може да е достатъчно нагоре за да е logn другите ще са (n/2)*0 + (n/4)*1 + (n/8)*2 .... 1*log(n).

Функция siftDown:
Извършва логаритмичен брой смненяния (swap) и в най-лошия случай височината на купчината е log(n). Следователно времевата сложност е O(log n).

heapsort функция:
Обажданията се увеличават, което отнема O(n) време.
Извършва n итерации, всяка от които включва операция с постоянно време и извикване на siftDown с времева сложност O(log n).
Като цяло времевата сложност на heapsort е O(n log n).*/