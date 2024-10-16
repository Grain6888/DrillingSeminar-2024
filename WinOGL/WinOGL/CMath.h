#pragma once
#include "CVertex.h"

class CMath {
public:
    // 2“_ŠÔ‚Ì‹——£‚ğŒvZ‚·‚é
    float VertexDistance (CVertex* p1, CVertex* p2);
    float VertexDistance (CVertex* p1, float p2_x, float p2_y);
    // ©Œğ·‚ğ”»’è‚·‚é
    bool SelfCross (CVertex* l1_p1, CVertex* l1_p2, CVertex* l2_p1, CVertex* l2_p2);
};

