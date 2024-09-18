#include <stdio.h>
#include "CStudent.h"

int main (void)
{
	// クラス変数の定義
	CStudent s1;
	CStudent s2 ('B', 90, 80, 80), s3 ('C', 30, 100, 70);

	s1.SetName ('A');
	s1.SetMath (100);
	s1.SetJapanese (60);
	s1.SetEnglish (30);

	printf ("%c さんの平均点は %.1f点です。\n", s1.GetName (), s1.CalcAverage ());
	printf ("%c さんの平均点は %.1f点です。\n", s2.GetName (), s2.CalcAverage ());
	printf ("%c さんの平均点は %.1f点です。\n", s3.GetName (), s3.CalcAverage ());

	return 0;
}