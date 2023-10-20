#include "vector.c"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int addNumbers(int n) {
  if (n != 0)
    return n + addNumbers(n - 1);
  else
    return n;
}

int power(int base, int a) {
    if (a != 0)
        return (base * power(base, a - 1));
    else
        return 1;
}

int recursiveSum(vector_t *v, int index) {
    if (index < 0) {
        return 0;
    }
    return v->arr[index] + recursiveSum(v, index - 1);
}

bool isVectorPalindrome(vector_t *v, int left, int right) {
    if (left >= right) {
        return true;
    }
    if (v->arr[left] != v->arr[right]) {
        return false;
    }
    return isVectorPalindrome(v, left + 1, right - 1);
}

int main() {
  int num = 5;
  printf("Sum = %d \n", addNumbers(num));

  int pow = 2;
  printf("%d^%d = %d  \n", num, pow, power(num, pow));

  vector_t *myVector = init_vector();

  push_back(myVector, 1);
  push_back(myVector, 2);
  push_back(myVector, 2);
  push_back(myVector, 1);

  printf("Vector elements: ");
  printVector(myVector);

  printf("Sum of vector elements: %d\n",  recursiveSum(myVector, myVector->size - 1));

    bool palindrome = isVectorPalindrome(myVector, 0, myVector->size - 1);
    
    if (palindrome) {
        printf("The vector is a palindrome.\n");
    } else {
        printf("The vector is not a palindrome.\n");
    }


  clear(myVector);

  return 0;
}