#include "vector.c"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int addNumbers(int n, int recNum) {
  if (n == 0)
    return recNum;
  else
    return addNumbers(n-1, recNum + n);
}

int powerSeccond(int base, int pow, int res) {
    if (pow == 0)
        return res;
    else
        return powerSeccond(base, pow-1, res * base);
}

int powerHelper(int base, int pow){
  return powerSeccond(base, pow, 1);
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
  printf("Sum = %d \n", addNumbers(num, 0));

  int pow = 2;
  printf("%d^%d = %d  \n", num, pow, powerHelper(num, pow));

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