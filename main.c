#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    FILE *file_in, *file_out;
    int n, m, i, j;

    clock_t start = clock();

    printf("enter count of rows: ");
    scanf("%d", &n);
    printf("enter count of columns: ");
    scanf("%d", &m);

    int total = n * m;

    file_in = fopen("input.txt", "w");


    srand(time(0));
    for (i = 0; i < total; i++) {
        int r = rand() % 100 + 1;
        fprintf(file_in, "%d ", r);
    }
    fclose(file_in);

    file_in = fopen("input.txt", "r");
    file_out = fopen("output.txt", "w");

    int num;
    while (fscanf(file_in, "%d", &num) != EOF) {
        if (num % 5 == 0 && num % 7 == 0) {
            fprintf(file_out, "FIVESEVEN ");
        } else if (num % 5 == 0) {
            fprintf(file_out, "FIVE ");
        } else if (num % 7 == 0) {
            fprintf(file_out, "SEVEN ");
        } else {
            fprintf(file_out, "%d ", num);
        }
    }

    fclose(file_in);
    fclose(file_out);


    file_in = fopen("input.txt", "r");
    int matrix[n][m];
    int rowsum[n], columnsum[m];

    for (i = 0; i < n; i++){
    	rowsum[i] = 0;
	} 
    for (j = 0; j < m; j++){
    	columnsum[j] = 0;
	}

    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            fscanf(file_in, "%d", &matrix[i][j]);
            rowsum[i] += matrix[i][j];
            columnsum[j] += matrix[i][j];
        }
    }
    fclose(file_in);

    printf("\n matrix:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
    
	printf("\nsum of row:\n");
	for (i = 0; i < n; i++){
		printf("%d\n", rowsum[i]);
	}
	
    printf("\nsum of column:\n");
    for (j = 0; j < m; j++) {
        printf("%4d", columnsum[j]);
    }
    printf("\n");

    clock_t end = clock();
    double alltime = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("time: %.2fsec\n", alltime);

    return 0;
}
