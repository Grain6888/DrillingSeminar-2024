#include <stdio.h>
#include "CVertex.h"

int main (void)
{
    CVertex s1;
    CVertex s2;
    CVertex s3(3, 3.50, 4.00);
    CVertex s4(4, 2.00, 3.60);

    s2.SetNumber(2);
    s2.SetX(5.05);

    s1.PrintVertex();
    s2.PrintVertex();
    s3.PrintVertex();
    s4.PrintVertex();

    return 0;
}