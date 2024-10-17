#pragma once
#include "CVertex.h"

class CMath {
public:
    // 2点間の距離を計算する
    float VertexDistance (CVertex* p1, CVertex* p2);
    float VertexDistance (CVertex* p1, float p2_x, float p2_y);
    // 自交差を判定する
    bool SelfCross (CVertex* a_s, CVertex* a_e, CVertex* b_s, CVertex* b_e);
    // 外積を計算する
    float OuterProduct (CVertex* p_a_s, CVertex* p_a_e, CVertex* p_b_s, CVertex* p_b_e);
    // ベクトルを計算する
    CVertex* CalcPointVector (CVertex* p_s, CVertex* p_e);
};

