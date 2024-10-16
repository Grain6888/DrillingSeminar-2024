#include "pch.h"
#include "CMath.h"
#include <math.h>

// 2点間の距離を計算する
float CMath::VertexDistance (CVertex* p1, CVertex* p2)
{
    return float (sqrt (pow ((p2->GetX () - p1->GetX ()), 2) + pow ((p2->GetY () - p1->GetY ()), 2)));
}
// 2点間の距離を計算する
float CMath::VertexDistance (CVertex* p1, float p2_x, float p2_y)
{
    return float (sqrt (pow ((p2_x - p1->GetX ()), 2) + pow ((p2_y - p1->GetY ()), 2)));
}

// 自交差を判定する
bool CMath::SelfCross (CVertex* l1_p1, CVertex* l1_p2, CVertex* l2_p1, CVertex* l2_p2)
{
    // TODO: ここに実装コードを追加します.
    return false;
}
