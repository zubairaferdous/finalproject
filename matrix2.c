#include <stdio.h>
#include <stdlib.h>

int main() {
    int r1 = 4, c1 = 5;
    int r2 = 5, c2 = 3;

    
    int **A = (int **)malloc(r1 * sizeof(int *));
    for (int i = 0; i < r1; i++) *(A + i) = (int *)malloc(c1 * sizeof(int));

    int **B = (int **)malloc(r2 * sizeof(int *));
    for (int i = 0; i < r2; i++) *(B + i) = (int *)malloc(c2 * sizeof(int));

    int **C = (int **)malloc(r1 * sizeof(int *));
    for (int i = 0; i < r1; i++) *(C + i) = (int *)malloc(c2 * sizeof(int));

   
    int count = 1;
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c1; j++) {
            *(*(A + i) + j) = count++;
        }
    }

   
    count = 1;
    for (int i = 0; i < r2; i++) {
        for (int j = 0; j < c2; j++) {
            *(*(B + i) + j) = count++;
        }
    }

  
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            *(*(C + i) + j) = 0; 
            for (int k = 0; k < c1; k++) {
                *(*(C + i) + j) += *(*(A + i) + k) * *(*(B + k) + j);
            }
        }
    }

   
    printf("Resultant 4x3 Matrix C:\n");
    printf("---------------------\n");
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            printf("%6d ", *(*(C + i) + j));
        }
        printf("\n");
    }

    
    for (int i = 0; i < r1; i++) free(*(A + i));
    free(A);
    for (int i = 0; i < r2; i++) free(*(B + i));
    free(B);
    for (int i = 0; i < r1; i++) free(*(C + i));
    free(C);

    return 0;
}