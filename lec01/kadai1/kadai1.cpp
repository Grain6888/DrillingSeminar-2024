#include<stdio.h>

int main (void)
{
    float num;

    printf ("’l‚ğ“ü—Í‚µ‚Ä‚­‚¾‚³‚¢F");
    scanf_s ("%f", &num);

    // ¬”‘æ“ñˆÊ‚ÅlÌŒÜ“ü
    // ‡@10‚ğŠ|‚¯‚é
    num = num * 10;

    // ‡A0.5‚ğ‰ÁZ‚·‚é
    num = num + 0.5;

    // ‡B¬”“_ˆÈ‰ºØ‚èÌ‚Ä
    num = (int)num;

    // ‡C10‚ÅŠ„‚é
    num = num / 10;

    printf ("lÌŒÜ“ü‚µ‚½’l‚Í%.2f‚Å‚·\n", num);

    return 0;
}