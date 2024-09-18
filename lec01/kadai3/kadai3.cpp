#include<stdio.h>

//べき乗を求める関数
float Power (float x, int n)
{
	//if (n == 0)
	//{
	//	return 1;
	//}
	//else
	//{
	//	return x * Power(x, --n);
	//}

	float result = 1;

	for (int i = n; i > 0; i--)
	{
		result *= x;
	}

	return result;
}

int main (void)
{
	int n;
	float x;

	printf ("実数を入力してください：");
	scanf_s ("%f", &x);
	printf ("整数を入力してください：");
	scanf_s ("%d", &n);

	//結果を表示
	printf ("%.2fの%d乗は%.2fです。\n", x, n, Power (x, n));

	return 0;
}