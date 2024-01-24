#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include "vector.h"

/*
 Ако искате компилирайте с MakeFile ако искате просто с gcc main.c heap.c vector.c
 Според мен по-лесно директно с команда
*/
//Напишете всичко други каквото се сещате за BST, предимства, недостатъци спрямо други алгоритми и т.н.
vector_t *create_vector_with_values()
{
    vector_t *v = init_vector();
    push_back(v, 5);
    push_back(v, 13);
    push_back(v, 23);
    push_back(v, 53);
    push_back(v, 56);
    push_back(v, 93);
    push_back(v, 100);
    push_back(v, -14);
    push_back(v, 0);
    push_back(v, 4);
    push_back(v, 55);
    push_back(v, 58);
    push_back(v, 123);

    return v;
}

//това може да не е вярно трябва да се върнаа!!!!
int find_k_smallest(vector_t *v, int k)
{

    for (int i = 0; i < k - 1; i++) {
        int root_value = v->arr[0];
        pop_root(v); 
        push_back(v, root_value);
    }

    return v->arr[0];
}


int main()
{
    vector_t *v = create_vector_with_values();

    heapify(v); // примерен правилен хийп 100 56 58 0 4 123 23 -14 53 13 55 93 5
    printVector(v);

    vector_t *v2 = create_vector_with_values();
    int k = 4;
    printf("Smallest %d in vector: %d", k, find_k_smallest(v2, k)); // 53 е

    clear(v);
    clear(v2);

    return 0;
}

/*

BST:

-Сложността на вмъкване в BST е O(log n) в средния случай и O(n) в най-лошия случай.

-Обхождането на BST във възходящ ред (in-order traversal) последователностр от сортирани елементите. Обхождането на BST има сложност O(n).

-Най-добър случай: O(n log n) - Когато дървото е балансирано и вмъкването винаги става в листата.
-Най-лош случай: O(n^2) - вмъкваме вече сортиран масив в BST


Заманата с вAVL:

Използването на AVL дърва  може да подобри най-лошия случай от O(n^2) на O(n log n), тъй като AVL дърветата се самобалансират.
Сложността на вмъкване и търсене в AVL дърво е O(log n) в най-лошия случай


Предимства:

-Позволява добавяне и премахване на елементи без да нарушава структурата на данните.

Недостатъци:

-В най-лошия случай може да бъде по-бавен от други алгоритми за сортиране, като quicksort или mergesort.
-Изисква допълнителна памет за съхранение на BST.


Сравнение:

-W  срванение с quicksort и mergesort обикновено е по-бавен в най-лошия случай, но има предимство пред тях при динамични операции върху данните
*/