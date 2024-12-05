#include "pch.h"
#include "CMath.h"

float CMath::VertexDis (CVertex* p1, CVertex* p2)
{
    return float (sqrt (pow ((p2->GetX () - p1->GetX ()), 2) + pow ((p2->GetY () - p1->GetY ()), 2)));
}

float CMath::LineDis (CVertex* p, CVertex* line_s, CVertex* line_e)
{
    // https://ikatakos.com/pot/programming_algorithm/geometry/point_to_line

    float ax = line_s->GetX ();
    float ay = line_s->GetY ();
    float bx = line_e->GetX ();
    float by = line_e->GetY ();
    float px = p->GetX ();
    float py = p->GetY ();

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

void CMath::CrossPoint (CVertex* p, CVertex* line_s, CVertex* line_e, CVertex* result)
{
    // https://ikatakos.com/pot/programming_algorithm/geometry/point_to_line

    float ax = line_s->GetX ();
    float ay = line_s->GetY ();
    float bx = line_e->GetX ();
    float by = line_e->GetY ();
    float px = p->GetX ();
    float py = p->GetY ();

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

    result->SetXY (cx, cy);
}

void CMath::ShiftPoint (CVertex* before, CVertex* after, CVertex* result)
{
    result->SetXY (result->GetLastX () + (after->GetX () - before->GetX ()), result->GetLastY () + (after->GetY () - before->GetY ()));
}

void CMath::ScalePoint (CVertex* before, CVertex* after, CVertex* result)
{
    float s_x = after->GetX () - before->GetX ();
    float s_y = after->GetY () - before->GetY ();

    result->SetXY (s_x * (result->GetLastX () - before->GetX ()) + before->GetLastX (), s_y * (result->GetLastY () - before->GetY ()) + before->GetLastY ());
}

void CMath::ScaleUpPoint (CVertex* base_p, CVertex* result)
{
    float s_x = 1.05f;
    float s_y = 1.05f;

    result->SetXY (s_x * (result->GetLastX () - base_p->GetX ()) + base_p->GetX (), s_y * (result->GetLastY () - base_p->GetY ()) + base_p->GetY ());
}

void CMath::ScaleDownPoint (CVertex* base_p, CVertex* result)
{
    float s_x = 0.95f;
    float s_y = 0.95f;

    float x = s_x * (result->GetLastX () - base_p->GetX ()) + base_p->GetX ();
    float y = s_y * (result->GetLastY () - base_p->GetY ()) + base_p->GetY ();

    result->SetXY (x, y);
}

void CMath::RotatePoint (float degree, CVertex* base_p, CVertex* result)
{
    float theta = (float)degree / 180 * M_PI;
    float x = (float)(result->GetLastX () - base_p->GetX ()) * cos (theta) - (result->GetLastY () - base_p->GetY ()) * sin (theta) + base_p->GetX ();
    float y = (float)(result->GetLastX () - base_p->GetX ()) * sin (theta) + (result->GetLastY () - base_p->GetY ()) * cos (theta) + base_p->GetY ();
    result->SetXY (x, y);
}

bool CMath::IsLineCrossing (CVertex* a_s, CVertex* a_e, CVertex* b_s, CVertex* b_e)
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

bool CMath::IsContained (CShape* my_shape, CVertex* new_vertex)
{
    double angle_sum = 0.0;
    for (CVertex* vp = my_shape->GetHead (); vp != my_shape->GetTail (); vp = vp->GetNext ())
    {
        angle_sum += VecAngle (new_vertex, vp, new_vertex, vp->GetNext ());
    }
    angle_sum += VecAngle (my_shape->GetTail (), new_vertex, my_shape->GetHead (), new_vertex);

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

bool CMath::IsContained (CShape* my_shape, CVertex* new_vertex, CVertex* skip_vertex)
{
    double angle_sum = 0.0;

    if (skip_vertex == my_shape->GetHead ())
    {
        for (CVertex* vp = my_shape->GetHead ()->GetNext (); vp != my_shape->GetTail () && vp != NULL; vp = vp->GetNext ())
        {
            angle_sum += VecAngle (new_vertex, vp, new_vertex, vp->GetNext ());
        }
        angle_sum += VecAngle (my_shape->GetTail (), new_vertex, my_shape->GetHead ()->GetNext (), new_vertex);
    }
    else if (skip_vertex == my_shape->GetTail ())
    {
        for (CVertex* vp = my_shape->GetHead (); vp != my_shape->GetTail () && vp != NULL; vp = vp->GetNext ())
        {
            if (vp == skip_vertex->GetPre ())
            {
                angle_sum += VecAngle (new_vertex, vp, new_vertex, my_shape->GetTail ());
                vp = vp->GetNext ();
            }
            else
            {
                angle_sum += VecAngle (new_vertex, vp, new_vertex, vp->GetNext ());
            }
        }
    }
    else
    {
        for (CVertex* vp = my_shape->GetHead (); vp != my_shape->GetTail () && vp != NULL; vp = vp->GetNext ())
        {
            if (vp == skip_vertex->GetPre ())
            {
                angle_sum += VecAngle (new_vertex, vp, new_vertex, vp->GetNext ()->GetNext ());
                vp = vp->GetNext ();
            }
            else
            {
                angle_sum += VecAngle (new_vertex, vp, new_vertex, vp->GetNext ());
            }
        }
        angle_sum += VecAngle (my_shape->GetTail (), new_vertex, my_shape->GetHead (), new_vertex);
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
