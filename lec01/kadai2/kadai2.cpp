#include<stdio.h>

int main (void)
{

	int num;

	printf ("”š‚ğˆê‚Â“ü—ÍF");
	scanf_s ("%d", &num);

	if (num > 0)
	{
		printf ("³‚Ì”‚Å‚·");
	}
	else if (num < 0)
	{
		printf ("•‰‚Ì”‚Å‚·");
	}
	else
	{
		printf ("0‚Å‚·");
	}

	return 0;
}