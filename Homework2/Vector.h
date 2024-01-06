#ifndef VECTOR_H
#define VECTOR_H

typedef void *TYPE;

typedef struct Vector {
    TYPE *data;
    int length;
    int capacity;
} Vector;

Vector *initVector();

void pushToVector(Vector *vector, TYPE value);

TYPE popFromVector(Vector *vector);

TYPE getFromVector(Vector *vector, int index);

void releaseVector(Vector *vector);

#endif