#include <stdio.h>
#include <stdlib.h>

int findMax(int arr[], int n) {
    int max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

void countingSort(int arr[], int n, int exp) {
    int output[n];
    int i, count[19] = {0};

    for (i = 0; i < n; i++) {
        count[(arr[i] / exp) % 10 + 9]++;
    }

    for (i = 1; i < 19; i++) {
        count[i] += count[i - 1];
    }

    for (i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10 + 9] - 1] = arr[i];
        count[(arr[i] / exp) % 10 + 9]--;
    }

    for (i = 0; i < n; i++) {
        arr[i] = output[i];
    }
}

void radixsort(int arr[], int n, int exp) {
    int max = findMax(arr, n);

    countingSort(arr, n, exp);

    if (max / exp != 0)
        radixsort(arr, n, exp * 10);
}

void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int arr[] = {170, -45, 75, -90, 802, -24, 2, 66, 778, 1, 2, 7, -2, -10, -23, - 90, -100, -802};
    int n = sizeof(arr) / sizeof(arr[0]);
    radixsort(arr, n, 1);
    printArray(arr, n);
    return 0;
}
