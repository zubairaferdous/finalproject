#include <stdio.h>

int temp[1000];  // temporary array (global, sized for max input)

// Merge arr[start..mid] and arr[mid+1..end] using temp[]
void merge(int arr[], int start, int mid, int end) {
    int i = start;      // pointer for left half
    int j = mid + 1;    // pointer for right half
    int k = start;      // pointer for temp array

    while (i <= mid && j <= end) {
        if (arr[i] <= arr[j]) {
            temp[k] = arr[i];
            i++;
        } else {
            temp[k] = arr[j];
            j++;
        }
        k++;
    }

    // Copy remaining elements of left half, if any
    while (i <= mid) {
        temp[k] = arr[i];
        i++;
        k++;
    }

    // Copy remaining elements of right half, if any
    while (j <= end) {
        temp[k] = arr[j];
        j++;
        k++;
    }

    // Copy merged elements back into original array
    for (int x = start; x <= end; x++) {
        arr[x] = temp[x];
    }
}

// Recursive mergesort function
void mergeSort(int arr[], int start, int end) {
    if (start < end) {
        int mid = start + (end - start) / 2;

        mergeSort(arr, start, mid);      // sort left half
        mergeSort(arr, mid + 1, end);    // sort right half

        merge(arr, start, mid, end);     // merge sorted halves
    }
}

// Utility function to print an array
void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main() {
    int arr[] = {12, 11, 13, 5, 6, 7};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Original array: ");
    printArray(arr, n);

    mergeSort(arr, 0, n - 1);

    printf("Sorted array:   ");
    printArray(arr, n);

    return 0;
}