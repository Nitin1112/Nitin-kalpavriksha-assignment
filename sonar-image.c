#include <stdio.h>

void generateMatrix(int *matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            *(matrix + i * n + j) = (i * 31 + j * 41 + 17) % 256;
        }
    }
}

void printMatrix(int *matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%3d ", *(matrix + i * n + j));
        }
        printf("\n");
    }
}

void rotate90Degree(int *matrix, int n)
{
    for (int i = 0; i < n / 2; i++)
    {
        for (int j = i; j < n - i - 1; j++)
        {
            int *top = matrix + i * n + j;
            int *right = matrix + j * n + (n - i - 1);
            int *bottom = matrix + (n - i - 1) * n + (n - j - 1);
            int *left = matrix + (n - j - 1) * n + i;

            int temp = *top;
            *top = *left;
            *left = *bottom;
            *bottom = *right;
            *right = temp;
        }
    }
}

void applySmoothingFilter(int *matrix, int n) {
    int prevRow[10], currRow[10], nextRow[10];

    for (int j = 0; j < n; j++) prevRow[j] = 0;

    for (int j = 0; j < n; j++) currRow[j] = *(matrix + 0 * n + j);

    if (n > 1) {
        for (int j = 0; j < n; j++) nextRow[j] = *(matrix + 1 * n + j);
    } else {
        for (int j = 0; j < n; j++) nextRow[j] = 0;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0, count = 0;
            for (int x = -1; x <= 1; x++) {      
                int r = i + x;
                int *rowPtr;
                if (r < 0) continue;              
                else if (r == i - 1) rowPtr = prevRow;
                else if (r == i) rowPtr = currRow;
                else if (r == i + 1) rowPtr = nextRow;
                else continue;                    

                for (int y = -1; y <= 1; y++) {  
                    int c = j + y;
                    if (c < 0 || c >= n) continue;
                    sum += rowPtr[c];
                    count++;
                }
            }
            *(matrix + i * n + j) = sum / count;  
        }

        for (int j = 0; j < n; j++) {
            prevRow[j] = currRow[j];
            currRow[j] = nextRow[j];
            if (i + 2 < n)
                nextRow[j] = *(matrix + (i + 2) * n + j);
            else
                nextRow[j] = 0;
        }
    }
}


int handleInput()
{
    int size;
    while (1)
    {
        printf("Enter matrix size (2-10): ");
        scanf("%d", &size);

        if (size >= 2 && size <= 10)
        {
            return size;
        }
        else
        {
            printf("Invalid size! Please enter a value between 2 and 10.\n");
        }
    }
}

void printOriginalMatrix()
{
    printf("\nOriginal Randomly Generated Matrix:\n");
}

void printRotatedMatrix()
{
    printf("\nMatrix after 90° Clockwise Rotation:\n");
}

void printFinalMatrix()
{
    printf("\nMatrix after Applying 3x3 Smoothing Filter:\n");
}

void processSonarImage()
{
    int n = handleInput();

    int matrix[n][n];
    generateMatrix((int *)matrix, n);
    printOriginalMatrix();
    printMatrix((int *)matrix, n);

    rotate90Degree((int *)matrix, n);
    printRotatedMatrix();
    printMatrix((int *)matrix, n);

    applySmoothingFilter((int *)matrix, n);
    printFinalMatrix();
    printMatrix((int *)matrix, n);
}

int main()
{
    while (1)
    {
        processSonarImage();
    }
    return 0;
}
