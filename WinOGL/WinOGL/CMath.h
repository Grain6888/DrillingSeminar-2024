#pragma once
#include "CVertex.h"
#include "CShape.h"

class CMath {
public:
    // 2点間の距離を計算する
    static float VertexDistance (CVertex* p1, CVertex* p2);
    static float VertexDistance (CVertex* p1, float p2_x, float p2_y);
    // 自交差を判定する
    static bool SelfCross (CVertex* a_s, CVertex* a_e, CVertex* b_s, CVertex* b_e);
    // 他交差を判定する
    static bool OtherCross (CShape* a_s, CShape* a_e, float new_x, float new_y);
    // 外積を計算する
    static float OuterProduct (CVertex* p_a_s, CVertex* p_a_e, CVertex* p_b_s, CVertex* p_b_e);
    // ベクトルを計算する
    static CVertex* CalcPointVector (CVertex* p_s, CVertex* p_e);
};

