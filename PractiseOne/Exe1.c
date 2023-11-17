#include "vector.h"
#include <stdlib.h>
#include <stdio.h>

vector_t *init_vector()
{
    vector_t *v = (vector_t *)malloc(sizeof(vector_t));
    v->arr = NULL;
    v->size = 0;
    v->capacity = 0;

    return v;
}

void resize(vector_t *v, int newCapacity)
{
    int *tmp = (int *)realloc(v->arr, newCapacity * sizeof(int));
    if (tmp == NULL)
    {
        exit(1);
    }

    v->arr = tmp;
    v->capacity = newCapacity;
}

void push_back(vector_t *v, int val)
{
    if (v->size == v->capacity)
    {
        if (v->capacity == 0)
        {
            v->capacity = 2;
        }
        else
        {
            v->capacity *= 2;
        }

        resize(v, v->capacity);
    }

    v->arr[v->size] = val;
    v->size++;
}

int pop(vector_t *v)
{
    if (v->size == 0)
    {
        return -1;
    }

    int val = v->arr[v->size - 1];
    v->size--;

    return val;
}

void clear(vector_t *v)
{
    free(v->arr);
    v->size = 0;
    v->capacity = 0;
}

int getAt(vector_t *v, int index)
{
    if (index >= v->size)
    {
        return -1;
        // exit(1);
    }

    return v->arr[index];
}

int getSize(vector_t *v)
{
    return v->size;
}

void printVector(vector_t *v)
{
    for (int i = 0; i < getSize(v); i++)
    {
        printf("%d ", getAt(v, i));
    }
    printf("\n");
}   


void sort(vector_t *v, int size) {
    size = v->size;

    for(int step = 1; step < size; step++){
        int key = v->arr[step];
        int j = step - 1;
        while(key < v->arr[j] && j>= 0){
            v->arr[j+1] = v->arr[j];
            --j;
        };
        v->arr[j+1] = key;
    }
}

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void selectionSort(vector_t*v, int size){
    int min_idx;
    for(int i = 0; i < size-1; i++){
        min_idx = i;
        for(int j = i+1; j < size; j++){
            if(v->arr[j] < v -> arr[min_idx]){
                min_idx = j;
            }
            if(min_idx != 1){
                swap(&v->arr[min_idx], &v->arr[i]);
            }
        }
    }
}


int main() {
    vector_t *v = init_vector();

    push_back(v, 5);
    push_back(v, 2);
    push_back(v, 8);
    push_back(v, 1);
    push_back(v, 9);  
    push_back(v, 10);
    push_back(v, 4);
    push_back(v, 200);
    push_back(v, -50);
    push_back(v, 0);
    push_back(v, -6 );

    printVector(v);

    selectionSort(v, getSize(v));

    printVector(v);;

    clear(v);

    free(v);

    return 0;
}