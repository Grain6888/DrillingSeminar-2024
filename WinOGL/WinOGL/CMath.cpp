#include "pch.h"
#include "CMath.h"

float CMath::VertexDis (CVertex* p1, CVertex* p2)
{
    return float (sqrt (pow ((p2->GetX () - p1->GetX ()), 2) + pow ((p2->GetY () - p1->GetY ()), 2)));
}

float CMath::LineDis (float v_x, float v_y, CVertex* sp_s, CVertex* sp_e)
{
    // https://ikatakos.com/pot/programming_algorithm/geometry/point_to_line

    float ax = sp_s->GetX ();
    float ay = sp_s->GetY ();
    float bx = sp_e->GetX ();
    float by = sp_e->GetY ();
    float px = v_x;
    float py = v_y;

    float abx = bx - ax;
    float aby = by - ay;
    float apx = px - ax;
    float apy = py - ay;

    float t = (apx * abx + apy * aby) / (powf (abx, 2) + powf (aby, 2));

    float cx = 0.0;
    float cy = 0.0;
    if (t <= 0)
    {
        cx = ax;
        cy = ay;
    }
    else if (t >= 1)
    {
        cx = bx;
        cy = by;
    }
    else
    {
        cx = ax + t * abx;
        cy = ay + t * aby;
    }
    return sqrtf ((powf ((px - cx), 2) + powf ((py - cy), 2)));
}

bool CMath::IsSelfCrossing (CVertex* a_s, CVertex* a_e, CVertex* b_s, CVertex* b_e)
{
    float outer_a_1 = Outer (a_s, a_e, a_s, b_s);
    float outer_a_2 = Outer (a_s, a_e, a_s, b_e);
    float outer_b_1 = Outer (b_s, b_e, b_s, a_s);
    float outer_b_2 = Outer (b_s, b_e, b_s, a_e);

    if ((outer_a_1 * outer_a_2 <= 0) && (outer_b_1 * outer_b_2 <= 0))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CMath::IsOtherCrossing (CShape* a_s, CShape* a_e, float new_x, float new_y)
{
    CVertex tmp_vertex (new_x, new_y, NULL, NULL);

    for (CShape* sp = a_s; sp != a_e; sp = sp->GetNext ())
    {
        // 図形の始点から終点までに存在する辺を対象に，他交差判定を行う．
        for (CVertex* vp = sp->GetHead (); vp != sp->GetTail (); vp = vp->GetNext ())
        {
            if (IsSelfCrossing (vp, vp->GetNext (), a_e->GetTail (), &tmp_vertex))
            {
                return true;
            }
        }
        // 図形の終点から始点に伸びる辺を対象に，他交差判定を行う．
        if (IsSelfCrossing (sp->GetHead (), sp->GetTail (), a_e->GetTail (), &tmp_vertex))
        {
            return true;
        }
    }
    return false;
}

bool CMath::IsOtherCrossing (CShape* a_s, CShape* a_e, CVertex* b_s, CVertex* b_e)
{

    for (CShape* sp = a_s; sp != a_e; sp = sp->GetNext ())
    {
        // 図形の始点から終点までに存在する辺を対象に，他交差判定を行う．
        for (CVertex* vp = sp->GetHead (); vp != sp->GetTail (); vp = vp->GetNext ())
        {
            if (IsSelfCrossing (vp, vp->GetNext (), b_s, b_e))
            {
                return true;
            }
        }
        // 図形の終点から始点に伸びる辺を対象に，他交差判定を行う．
        if (IsSelfCrossing (sp->GetHead (), sp->GetTail (), b_s, b_e))
        {
            return true;
        }
    }
    return false;
}

bool CMath::IsInclusion (CShape* a_s, CShape* a_e, float new_x, float new_y)
{
    double angle_sum = 0.0;
    CVertex* new_p = new CVertex;
    new_p->SetXY (new_x, new_y);

    for (CShape* sp = a_s; sp != a_e->GetNext (); sp = sp->GetNext ())
    {
        for (CVertex* vp = sp->GetHead (); vp != sp->GetTail (); vp = vp->GetNext ())
        {
            angle_sum += VecAngle (new_p, vp, new_p, vp->GetNext ());
        }
        angle_sum += VecAngle (sp->GetTail (), new_p, sp->GetHead (), new_p);
    }

    new_p->FreeVertex ();
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

float CMath::Inner (CVertex* p_a_s, CVertex* p_a_e, CVertex* p_b_s, CVertex* p_b_e)
{
    CVertex* v_a = PositionVec (p_a_s, p_a_e);
    CVertex* v_b = PositionVec (p_b_s, p_b_e);

    float result = v_a->GetX () * v_b->GetX () + v_a->GetY () * v_b->GetY ();
    v_a->FreeVertex ();
    v_b->FreeVertex ();
    return result;
}

float CMath::Outer (CVertex* p_a_s, CVertex* p_a_e, CVertex* p_b_s, CVertex* p_b_e)
{
    CVertex* v_a = PositionVec (p_a_s, p_a_e);
    CVertex* v_b = PositionVec (p_b_s, p_b_e);

    float result = v_a->GetX () * v_b->GetY () - v_a->GetY () * v_b->GetX ();
    v_a->FreeVertex ();
    v_b->FreeVertex ();
    return result;
}

CVertex* CMath::PositionVec (CVertex* p_s, CVertex* p_e)
{
    CVertex* new_point_vector = new CVertex;
    new_point_vector->SetXY (p_e->GetX () - p_s->GetX (), p_e->GetY () - p_s->GetY ());
    return new_point_vector;
}

double CMath::VecAngle (CVertex* p_a_s, CVertex* p_a_e, CVertex* p_b_s, CVertex* p_b_e)
{
    float outer_product = Outer (p_a_s, p_a_e, p_b_s, p_b_e);
    float inner_product = Inner (p_a_s, p_a_e, p_b_s, p_b_e);
    return atan2 (outer_product, inner_product);
}
