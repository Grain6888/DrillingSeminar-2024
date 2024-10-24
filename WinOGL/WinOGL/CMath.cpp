#include "pch.h"
#include "CMath.h"

float CMath::VertexDistance (CVertex* p1, CVertex* p2)
{
    return float (sqrt (pow ((p2->GetX () - p1->GetX ()), 2) + pow ((p2->GetY () - p1->GetY ()), 2)));
}

float CMath::VertexDistance (CVertex* p1, float p2_x, float p2_y)
{
    return float (sqrt (pow ((p2_x - p1->GetX ()), 2) + pow ((p2_y - p1->GetY ()), 2)));
}

bool CMath::CrossDetect (CVertex* a_s, CVertex* a_e, CVertex* b_s, CVertex* b_e)
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

bool CMath::OtherCrossDetect (CShape* a_s, CShape* a_e, float new_x, float new_y)
{
    CVertex* new_p = new CVertex;
    new_p->SetXY (new_x, new_y);

    for (CShape* sp = a_s; sp != a_e; sp = sp->GetNext ())
    {
        // 図形の始点から終点までに存在する辺を対象に，他交差判定を行う．
        for (CVertex* vp = sp->GetHead (); vp != sp->GetTail (); vp = vp->GetNext ())
        {
            if (CrossDetect (vp, vp->GetNext (), a_e->GetTail (), new_p))
            {
                new_p->FreeVertex ();
                return true;
            }
        }
        // 図形の終点から始点に伸びる辺を対象に，他交差判定を行う．
        if (CrossDetect (sp->GetHead (), sp->GetTail (), a_e->GetTail (), new_p))
        {
            new_p->FreeVertex ();
            return true;
        }
    }
    new_p->FreeVertex ();
    return false;
}

bool CMath::InclusionDetect (CShape* a_s, CShape* a_e, float new_x, float new_y)
{
    double angle_sum = 0.0;
    CVertex* new_p = new CVertex;
    new_p->SetXY (new_x, new_y);

    for (CShape* sp = a_s; sp != a_e; sp = sp->GetNext ())
    {
        for (CVertex* vp = sp->GetHead (); vp != sp->GetTail (); vp = vp->GetNext ())
        {
            angle_sum += CalcAngle (new_p, vp, new_p, vp->GetNext ());
        }
        angle_sum += CalcAngle (sp->GetTail (), new_p, sp->GetHead (), new_p);
    }

    double difference = 2 * M_PI - fabs (angle_sum);
    if (difference >= -0.001 && difference <= 0.001)
    {
        return true;
    }
    else
    {
        return false;
    }
}

float CMath::InnerProduct (CVertex* p_a_s, CVertex* p_a_e, CVertex* p_b_s, CVertex* p_b_e)
{
    CVertex* v_a = CalcPointVector (p_a_s, p_a_e);
    CVertex* v_b = CalcPointVector (p_b_s, p_b_e);

    float result = v_a->GetX () * v_b->GetX () + v_a->GetY () * v_b->GetY ();
    v_a->FreeVertex ();
    v_b->FreeVertex ();
    return result;
}

float CMath::OuterProduct (CVertex* p_a_s, CVertex* p_a_e, CVertex* p_b_s, CVertex* p_b_e)
{
    CVertex* v_a = CalcPointVector (p_a_s, p_a_e);
    CVertex* v_b = CalcPointVector (p_b_s, p_b_e);

    float result = v_a->GetX () * v_b->GetY () - v_a->GetY () * v_b->GetX ();
    v_a->FreeVertex ();
    v_b->FreeVertex ();
    return result;
}

CVertex* CMath::CalcPointVector (CVertex* p_s, CVertex* p_e)
{
    CVertex* new_point_vector = new CVertex;
    new_point_vector->SetXY (p_e->GetX () - p_s->GetX (), p_e->GetY () - p_s->GetY ());
    return new_point_vector;
}

double CMath::CalcAngle (CVertex* p_a_s, CVertex* p_a_e, CVertex* p_b_s, CVertex* p_b_e)
{
    float outer_product = OuterProduct (p_a_s, p_a_e, p_b_s, p_b_e);
    float inner_product = InnerProduct (p_a_s, p_a_e, p_b_s, p_b_e);
    return atan2 (outer_product, inner_product);
}
