#include "Vector.h"
#include <stdlib.h>

Vector *initVector() {
    Vector *vector = (Vector *)malloc(sizeof(Vector));

    vector->data = NULL;
    vector->length = 0;
    vector->capacity = 1;

    return vector;
}

void static resizeVector(Vector *vector) {
    if (vector->length > vector->capacity) {
        vector->capacity *= 2;
        vector->data = (TYPE *)realloc(vector->data, sizeof(TYPE) * vector->capacity);
    } else if (vector->length < vector->capacity / 2) {
        vector->capacity /= 2;
        vector->data = (TYPE *)realloc(vector->data, sizeof(TYPE) * vector->capacity);
    }
}

void pushToVector(Vector *vector, TYPE value) {
    resizeVector(vector);

    vector->data = (TYPE *)realloc(vector->data, sizeof(TYPE) * (vector->length + 1));

    vector->data[vector->length++] = value;
}

TYPE popFromVector(Vector *vector) {
    if (vector->length == 0)
        return NULL;

    TYPE value = vector->data[vector->length - 1];

    vector->data = (TYPE *)realloc(vector->data, sizeof(TYPE) * (vector->length - 1));

    vector->length--;

    resizeVector(vector);

    return value;
}

TYPE getFromVector(Vector *vector, int index) {
    if (index < 0 || index >= vector->length)
        return NULL;

    return vector->data[index];
}

void releaseVector(Vector *vector) {
    for (int i = 0; i < vector->length; i++) {
        free(vector->data[i]);
    }

    free(vector->data);
    free(vector);
}