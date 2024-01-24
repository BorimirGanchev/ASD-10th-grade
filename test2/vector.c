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
        printf("Cannot pop empty vector.");
        exit(1);
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
    if (index < 0 || (unsigned)index >= v->size)
    {
        printf("Invalid index for getAt");
        exit(1);
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
