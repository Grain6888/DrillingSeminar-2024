#include "pch.h"
#include "CMath.h"
#include <math.h>

float CMath::VertexDistance (CVertex* p1, CVertex* p2)
{
    return float (sqrt (pow ((p2->GetX () - p1->GetX ()), 2) + pow ((p2->GetY () - p1->GetY ()), 2)));
}

float CMath::VertexDistance (CVertex* p1, float p2_x, float p2_y)
{
    return float (sqrt (pow ((p2_x - p1->GetX ()), 2) + pow ((p2_y - p1->GetY ()), 2)));
}