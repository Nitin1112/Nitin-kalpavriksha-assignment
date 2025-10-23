#include <stdio.h>
#include <stdbool.h>

#define NUMBER_LIMIT 256
#define PRIME_1 31
#define PRIME_2 41
#define PRIME_3 17
#define MAX_SIZE 10

int randomValue(int row_value, int column_value)
{
    return (row_value * PRIME_1 + column_value * PRIME_2 + PRIME_3) % NUMBER_LIMIT;
}
void generateMatrix(int *matrix, int size)
{
    for (int idx_row = 0; idx_row < size; idx_row++)
    {
        for (int idx_column = 0; idx_column < size; idx_column++)
        {
            *(matrix + idx_row * size + idx_column) = randomValue(idx_row, idx_column);
        }
    }
}

void printMatrix(int *matrix, int size)
{
    for (int idx_row = 0; idx_row < size; idx_row++)
    {
        for (int idx_column = 0; idx_column < size; idx_column++)
        {
            printf("%3d ", *(matrix + idx_row * size + idx_column));
        }
        printf("\n");
    }
}

void rotate90Clockwise(int *matrix, int size)
{
    for (int idx_row = 0; idx_row < size / 2; idx_row++)
    {
        for (int idx_column = idx_row; idx_column < size - idx_row - 1; idx_column++)
        {
            int *top = matrix + idx_row * size + idx_column;
            int *right = matrix + idx_column * size + (size - idx_row - 1);
            int *bottom = matrix + (size - idx_row - 1) * size + (size - idx_column - 1);
            int *left = matrix + (size - idx_column - 1) * size + idx_row;

            int temp = *top;
            *top = *left;
            *left = *bottom;
            *bottom = *right;
            *right = temp;
        }
    }
}

void computeNeighborhoodSum(int idx_row, int idx_column, int size, int *sum, int *count, int *prevRow, int *currRow, int *nextRow)
{
    for (int x = -1; x <= 1; x++)
    {
        int r = idx_row + x;
        int *rowPtr;
        if (r < 0)
            continue;
        else if (r == idx_row - 1)
            rowPtr = prevRow;
        else if (r == idx_row)
            rowPtr = currRow;
        else if (r == idx_row + 1)
            rowPtr = nextRow;
        else
            continue;

        for (int y = -1; y <= 1; y++)
        {
            int c = idx_column + y;
            if (c < 0 || c >= size)
                continue;
            *sum += rowPtr[c];
            (*count)++;
        }
    }
}

void updateRows(int *matrix, int size, int idx_row, int *prevRow, int *currRow, int *nextRow)
{
    for (int idx = 0; idx < size; idx++)
    {
        prevRow[idx] = currRow[idx];
        currRow[idx] = nextRow[idx];
        if (idx_row + 2 < size)
            nextRow[idx] = *(matrix + (idx_row + 2) * size + idx);
        else
            nextRow[idx] = 0;
    }
}

void applySmoothingFilter(int *matrix, int size)
{
    int prevRow[MAX_SIZE], currRow[MAX_SIZE], nextRow[MAX_SIZE];

    for (int idx = 0; idx < size; idx++)
        prevRow[idx] = 0;

    for (int idx = 0; idx < size; idx++)
        currRow[idx] = *(matrix + 0 * size + idx);

    if (size > 1)
    {
        for (int idx = 0; idx < size; idx++)
            nextRow[idx] = *(matrix + 1 * size + idx);
    }
    else
    {
        for (int idx = 0; idx < size; idx++)
            nextRow[idx] = 0;
    }

    for (int idx_row = 0; idx_row < size; idx_row++)
    {
        for (int idx_column = 0; idx_column < size; idx_column++)
        {
            int sum = 0, count = 0;
            computeNeighborhoodSum(idx_row, idx_column, size, &sum, &count, prevRow, currRow, nextRow);
            *(matrix + idx_row * size + idx_column) = sum / count;
        }
        updateRows(matrix, size, idx_row, prevRow, currRow, nextRow);
    }
}

bool checkRange(int size)
{
    if (size < 2 || size > 10)
    {
        return false;
    }
    return true;
}

int handleInput()
{
    int size;
    while (1)
    {
        printf("Enter matrix size (2-10): ");
        scanf("%d", &size);

        if (checkRange(size))
        {
            return size;
        }
        else
        {
            printf("Invalid size! Please enter a value between 2 and 10.\n");
        }
    }
}

void processSonarImage()
{
    int size = handleInput();

    int matrix[size][size];
    generateMatrix((int *)matrix, size);
    printf("\nOriginal Randomly Generated Matrix:\n");
    printMatrix((int *)matrix, size);

    rotate90Degree((int *)matrix, size);
    printf("\nMatrix after 90° Clockwise Rotation:\n");
    printMatrix((int *)matrix, size);

    applySmoothingFilter((int *)matrix, size);
    printf("\nMatrix after Applying 3x3 Smoothing Filter:\n");
    printMatrix((int *)matrix, size);
}

int main()
{
    while (1)
    {
        processSonarImage();
    }
    return 0;
}
