#include "vector.h"
#include <stdlib.h>
#include <stdio.h>

/*
    Бихме могли да върнем и просто вектор а не указател.
    vector_t init_vector();

    По стандартно е да се работи с указатели. Освен това ако върнем директно
    структура то данните на структурата трябва да се копират в резултата.
    Тоест при използване:
    vector_t v = init_vector();

    резултатът от init_vector() трябва да се копира във v. В случая няма да има много разлика, но
    ако беше по-сложна и голяма структура можем да си го спестим като само подаваме указатели.

    Друга малка причина е че в останалите функции така или иначе се работи само с указатели.
    Иначе трябва навсякъде да подаваме с &v вместо само v.
*/
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
    /*
        Реалокирането на памет е операция, която искаме да избегнем да правим на всяко добавяне на стойност.
        Затова в нашата имплементация на вектор добавяме и капацитет, който указва количеството заделена памет,
        която имаме на разположение, а size показва частта, която реално използваме в момента.
        Така може да си позволим да реалокираме само когато size достигне capacity. Умножавайки capacity по някаква
        константа като 2 (така работи и стандардния вектор в C++) или 1.5 си гарантираме, че няма да се налага
        да реалокираме при всяко добавяне на елемент.
        Да, това ще доведе до излишество на памет, така че ако искаме максимално оптимални с паметта няма нужда от
        capacity и бихме реалокирали всеки път. Но в повечето случаи бързодействието е по-приоритетно от паметта,
        особено когато излишеството не е голямо както в случая.
    */
    if (v->size == v->capacity)
    {
        // можеше да зададем някакъв начален капацитет за да не се налага да правим тази проверка
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

/*
    pop не е стандардна функция за вектор. Повече се използва за други структури като опашка и стек, които ще разгледаме по-късно,
    но понеже някой го спомена направихме и нея.
*/
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


int main() {
    vector_t *v = init_vector();

    push_back(v, 5);
    push_back(v, 2);
    push_back(v, 8);
    push_back(v, 1);
    push_back(v, 9);
    push_back(v, -4);   
    push_back(v, 10);
    push_back(v, 200);
    push_back(v, -50);
    push_back(v, 0);
    push_back(v, -6 );

    printVector(v);

    sort(v, getSize(v));

    printVector(v);;

    clear(v);

    free(v);

    return 0;
}