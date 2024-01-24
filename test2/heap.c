#include <stdio.h>
#include "vector.h"

/*В heap.c има започната имплементация на max-heap. Довършете функциите siftUp и siftDown. Тя трябва да взимат елемент на позиция i и да го siftUp/down-ва до правилното му място с модификацията, че няма да гледаме елементите строго по-големина. Вместо това четните са винаги с предимство и се смятат за по-големи. Тоест при съставяне на max heap от числата {1,2,3,4,5,6,7, 8,  9} един възможен heap би бил: {8, 6, 4, 9, 2, 5, 1, 7, 3} . Използвайте някоя от двете функции за да построите heap спазващ условията за четни и нечетни. Бонус: обяснете каква е разликата между създаване на heap със siftUp и siftDown */


void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void siftUp(vector_t *v, int i)
{
    while (i > 0) {
        int parent = (i - 1) / 2;
        //Трябва да се върнза, защото не се харесва!!!!!!!
        if ((v->arr[i] % 2 == 0 && v->arr[parent] % 2 == 0 && v->arr[i] > v->arr[parent]) ||(v->arr[i] % 2 != 0 && v->arr[parent] % 2 != 0 && v->arr[i] > v->arr[parent])) {
            swap(&v->arr[i], &v->arr[parent]);
            i = parent;
        } else {
            break;
        }
    }
}

void siftDown(vector_t *v, int i)
{
    int max_index = i;
    int left_child = 2 * i + 1;
    int right_child = 2 * i + 2;
    
    if (left_child < v->size && ((v->arr[left_child] % 2 == 0 && v->arr[max_index] % 2 == 0 && v->arr[left_child] > v->arr[max_index]) || (v->arr[left_child] % 2 != 0 && v->arr[max_index] % 2 != 0 && v->arr[left_child] > v->arr[max_index]))) {
        max_index = left_child;
    }

    if (right_child < v->size && ((v->arr[right_child] % 2 == 0 && v->arr[max_index] % 2 == 0 && v->arr[right_child] > v->arr[max_index]) || (v->arr[right_child] % 2 != 0 && v->arr[max_index] % 2 != 0 && v->arr[right_child] > v->arr[max_index]))) {
        max_index = right_child;
    }

    if (i != max_index) {
        swap(&v->arr[i], &v->arr[max_index]);
        siftDown(v, max_index);
    }
}

/*.

siftUp:

- При siftUp  добавяме на нов елемент в края на масива и след това "променяме" масива, като го подреждаме така, че да запазва свойствата на хийпа (например, max-heap).

soiftDown
- Началата идея при siftDown е вземане на коренов елемент (най-големия в max-heap) и след това преустройване на хийпа, за да се възстановят характеристиките му след премахването на кореновия елемент.

siftUp работи отдолу нагоре (от новия елемент към корена), докато siftDown работи отгоре надолу.
siftUp обикновено се използва при добавяне на нови елементи в хийп, докато siftDown се използва след премахване на кореновия елемент.
siftUp изисква постоянно обмяна на елементи, като новият елемент се движи към корена, докато siftDown изисква обичайно обмяна само с по-голямите от двете деца..*/

void heapify(vector_t *v)
{
    for (int i = (v->size - 1) / 2; i >= 0; i--) {
        siftDown(v, i);
    }
}
