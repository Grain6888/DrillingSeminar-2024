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
bool CMath::SelfCross (CVertex* a_s, CVertex* a_e, CVertex* b_s, CVertex* b_e)
{
    float outer_product_a_1 = OuterProduct (a_s, a_e, a_s, b_s);
    float outer_product_a_2 = OuterProduct (a_s, a_e, a_s, b_e);
    float outer_product_b_1 = OuterProduct (b_s, b_e, b_s, a_s);
    float outer_product_b_2 = OuterProduct (b_s, b_e, b_s, a_e);

    if ((outer_product_a_1 * outer_product_a_2 <= 0) && (outer_product_b_1 * outer_product_b_2 <= 0))
    {
        return true;
    }
    else
    {
        return false;
    }
}

// 外積を計算する
float CMath::OuterProduct (CVertex* p_a_s, CVertex* p_a_e, CVertex* p_b_s, CVertex* p_b_e)
{
    CVertex* v_a = CalcPointVector (p_a_s, p_a_e);
    CVertex* v_b = CalcPointVector (p_b_s, p_b_e);

    return v_a->GetX () * v_b->GetY () - v_a->GetY () * v_b->GetX ();
}

// 位置ベクトルを計算する
CVertex* CMath::CalcPointVector (CVertex* p_s, CVertex* p_e)
{
    CVertex* new_point_vector = new CVertex;
    new_point_vector->SetXY (p_e->GetX () - p_s->GetX (), p_e->GetX () - p_s->GetY ());
    return new_point_vector;
}