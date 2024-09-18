#include <stdio.h>

#define MAX_ROW 9
#define MAX_COL 9

void showArray(int array[MAX_ROW][MAX_COL], int row, int col);
void fillTimeTable(int array[MAX_ROW][MAX_COL], int row, int col);

int main (void)
{
    int array[9][9];
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            array[i][j] = j + 1;
        }
    }
    printf("‰Šú”z—ñ\n");
    showArray(array, 9, 9);

    fillTimeTable(array, 9, 9);
    printf("‹ã‹ãŒvŽZŒ‹‰Ê\n");
    showArray(array, 9, 9);
}

void showArray (int array[MAX_ROW][MAX_COL], int row, int col)
{
    for (int i = 0; i < col; i++)
    {
        for (int j = 0; j < row; j++)
        {
            printf("%3d", array[i][j]);
        }
        printf("\n");
    }
}

void fillTimeTable (int array[MAX_ROW][MAX_COL], int row, int col)
{
    for (int i = 0; i < col; i++)
    {
        for (int j = 0; j < row; j++)
        {
            array[i][j] = (i+1)*(j+1);
        }
    }
}