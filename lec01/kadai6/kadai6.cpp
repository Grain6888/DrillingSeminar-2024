#include <stdio.h>

void showArray(int array[], int n);
int Sum(int array[], int n);

int main (void)
{
    int array[10];

    for (int i = 0; i < 10; i++)
    {
        array[i] = i;
    }
    showArray(array, 10);

    printf("‡Œv’l: %d", Sum(array, 10));
}

void showArray (int array[], int n)
{
    printf ("{ ");
    for (int i = 0; i < n; i++)
    {
        printf ("%d ", array[i]);
    }
    printf ("}\n");
}

int Sum (int array[], int n)
{
    int result = 0;

    for (int i = 0; i < n; i++)
    {
        result += array[i];
    }

    return result;
}