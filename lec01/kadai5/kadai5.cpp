#include <stdio.h>

void showArray (int array[], int n);
void Absolute (int array[], int n);

int main (void)
{
    int array[10] = { -13, 0, 4, -5, -1, 11, -7, 1, -4, -16 };

    printf("変換前の配列の要素\n");
    showArray(array, 10);

    Absolute(array, 10);

    printf("変換後の配列の要素\n");
    showArray(array, 10);
}

void showArray (int array[], int n)
{
    printf("{ ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", array[i]);
    }
    printf("}\n");
}

void Absolute (int array[], int n)
{
    for (int i = 0; i < n; i++)
    {
        if (array[i] < 0)
        {
            array[i] *= -1;
        }
    }
}