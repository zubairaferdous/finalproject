#include <stdio.h>
#include <stdlib.h>
int main() {
    int row = 4, col = 5;
    int **matrix = (int **)malloc(row * sizeof(int *));
    for (int i = 0; i < row; i++) {
        *(matrix + i) = (int *)malloc(col * sizeof(int));
    }
    int k = 0;
    for (int i = 0; i < row; i++) {
        int *temp = *(matrix + i);
        for (int j = 0; j < col; j++){
           printf("%d" , *(temp + j)); 
        
        }
        printf("\n");
    }
    for(int i = 0; i < row; i++) {
        free(*(matrix + i));
    }
    free(matrix);
}