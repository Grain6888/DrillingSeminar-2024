#include<stdio.h>

//プロトタイプ宣言
float Power(float x, int n);

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

//べき乗を求める関数
float Power (float x, int n)
{
	float result = 1;

	while (n > 0)
	{
		result *= x;
		n--;
	}

	return result;
}