#include "pch.h"
#include "CMath.h"

float CMath::VertexDis (CVertex* p1, CVertex* p2)
{
    float x_dis = powf ((p2->GetX () - p1->GetX ()), 2);
    float y_dis = powf ((p2->GetY () - p1->GetY ()), 2);
    return sqrtf (x_dis + y_dis);
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

    float x_dis = powf ((px - cx), 2);
    float y_dis = powf ((py - cy), 2);
    return sqrtf (x_dis + y_dis);
}

float CMath::TriangleArea (CShape* triangle)
{
    CVertex* v_a = PositionVec (triangle->GetHead (), triangle->GetHead ()->GetNext ());
    CVertex* v_b = PositionVec (triangle->GetHead (), triangle->GetTail ());

    float size_2_v_a = powf (v_a->GetX (), 2) + powf (v_a->GetY (), 2);
    float size_2_v_b = powf (v_b->GetX (), 2) * powf (v_b->GetY (), 2);
    float inner_a_b = Inner (triangle->GetHead (), triangle->GetHead ()->GetNext (), triangle->GetHead (), triangle->GetTail ());

    float length_a = sqrtf (size_2_v_a);
    float length_b = sqrtf (size_2_v_b);

    float cos_theta = inner_a_b / (length_a * length_b);
    float sin_theta = sqrtf (1 - powf (cos_theta, 2));

    float area = 0.5 * length_a * length_b * sin_theta;

    v_a->FreeVertex ();
    v_b->FreeVertex ();
    //return sqrtf (size_2_v_a * size_2_v_b - powf (inner_a_b, 2)) / 2;
    return area;
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

    GLfloat cx = 0.0;
    GLfloat cy = 0.0;
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

void CMath::GravityPoint (CShape* my_shape, CVertex* result)
{
    GLfloat sum_x = 0.0;
    GLfloat sum_y = 0.0;

    for (CVertex* vp = my_shape->GetHead (); vp != NULL; vp = vp->GetNext ())
    {
        sum_x += vp->GetX ();
        sum_y += vp->GetY ();
    }

    result->SetXY (sum_x / my_shape->GetVertexNum (), sum_y / my_shape->GetVertexNum ());
}

void CMath::ShiftPoint (CVertex* before, CVertex* after, CVertex* result)
{
    result->SetXY (result->GetLastX () + (after->GetX () - before->GetX ()), result->GetLastY () + (after->GetY () - before->GetY ()));
}

void CMath::ScalePoint (CVertex* base_p, CVertex* before, CVertex* after, CVertex* result)
{
    float s_x = (after->GetX () - base_p->GetX ()) / (before->GetX () - base_p->GetX ());
    float s_y = (after->GetY () - base_p->GetY ()) / (before->GetY () - base_p->GetY ());

    GLfloat x = s_x * (result->GetLastX () - base_p->GetX ()) + base_p->GetX ();
    GLfloat y = s_y * (result->GetLastY () - base_p->GetY ()) + base_p->GetY ();

    result->SetXY (x, y);
}

void CMath::ScalePoint (float scale, CVertex* base_p, CVertex* result)
{
    GLfloat x = scale * (result->GetLastX () - base_p->GetX ()) + base_p->GetX ();
    GLfloat y = scale * (result->GetLastY () - base_p->GetY ()) + base_p->GetY ();

    result->SetXY (x, y);
}

void CMath::RotatePoint (CVertex* base_p, CVertex* before, CVertex* after, CVertex* result)
{
    float theta = VecAngle (base_p, before, base_p, after);
    GLfloat x = (result->GetLastX () - base_p->GetX ()) * cosf (theta) - (result->GetLastY () - base_p->GetY ()) * sinf (theta) + base_p->GetX ();
    GLfloat y = (result->GetLastX () - base_p->GetX ()) * sinf (theta) + (result->GetLastY () - base_p->GetY ()) * cosf (theta) + base_p->GetY ();
    result->SetXY (x, y);
}

void CMath::RotatePoint (float degree, CVertex* base_p, CVertex* result)
{
    float theta = degree / 180 * (float)M_PI;
    GLfloat x = (result->GetLastX () - base_p->GetX ()) * cosf (theta) - (result->GetLastY () - base_p->GetY ()) * sinf (theta) + base_p->GetX ();
    GLfloat y = (result->GetLastX () - base_p->GetX ()) * sinf (theta) + (result->GetLastY () - base_p->GetY ()) * cosf (theta) + base_p->GetY ();
    result->SetXY (x, y);
}

bool CMath::IsLineCrossing (CVertex* a_s, CVertex* a_e, CVertex* b_s, CVertex* b_e)
{
    float outer_a_1 = Outer2DSize (a_s, a_e, a_s, b_s);
    float outer_a_2 = Outer2DSize (a_s, a_e, a_s, b_e);
    float outer_b_1 = Outer2DSize (b_s, b_e, b_s, a_s);
    float outer_b_2 = Outer2DSize (b_s, b_e, b_s, a_e);

    if ((outer_a_1 * outer_a_2 < 0) && (outer_b_1 * outer_b_2 < 0))
    {
        return true;
    }

    if (outer_a_1 == 0)
    {
        if ((b_s->GetX () >= (std::min)(a_s->GetX (), a_e->GetX ()) && b_s->GetX () <= (std::max)(a_s->GetX (), a_e->GetX ())) &&
            (b_s->GetY () >= (std::min)(a_s->GetY (), a_e->GetY ()) && b_s->GetY () <= (std::max)(a_s->GetY (), a_e->GetY ())))
        {
            return true;
        }
    }
    if (outer_a_2 == 0)
    {
        if ((b_e->GetX () >= (std::min)(a_s->GetX (), a_e->GetX ()) && b_e->GetX () <= (std::max)(a_s->GetX (), a_e->GetX ())) &&
            (b_e->GetY () >= (std::min)(a_s->GetY (), a_e->GetY ()) && b_e->GetY () <= (std::max)(a_s->GetY (), a_e->GetY ())))
        {
            return true;
        }
    }
    if (outer_b_1 == 0)
    {
        if ((a_s->GetX () >= (std::min)(b_s->GetX (), b_e->GetX ()) && a_s->GetX () <= (std::max)(b_s->GetX (), b_e->GetX ())) &&
            (a_s->GetY () >= (std::min)(b_s->GetY (), b_e->GetY ()) && a_s->GetY () <= (std::max)(b_s->GetY (), b_e->GetY ())))
        {
            return true;
        }
    }
    if (outer_b_2 == 0)
    {
        if ((a_e->GetX () >= (std::min)(b_s->GetX (), b_e->GetX ()) && a_e->GetX () <= (std::max)(b_s->GetX (), b_e->GetX ())) &&
            (a_e->GetY () >= (std::min)(b_s->GetY (), b_e->GetY ()) && a_e->GetY () <= (std::max)(b_s->GetY (), b_e->GetY ())))
        {
            return true;
        }
    }
    return false;
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

bool CMath::IsReversed (CShape* my_shape)
{
    double angle_sum = 0.0;
    CVertex* g = new CVertex;
    GravityPoint (my_shape, g);
    for (CVertex* vp = my_shape->GetHead (); vp != my_shape->GetTail (); vp = vp->GetNext ())
    {
        angle_sum += VecAngle (g, vp, g, vp->GetNext ());
    }
    angle_sum += VecAngle (my_shape->GetTail (), g, my_shape->GetHead (), g);

    g->FreeVertex ();
    if (angle_sum > 0)
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

float CMath::Outer2DSize (CVertex* p_a_s, CVertex* p_a_e, CVertex* p_b_s, CVertex* p_b_e)
{
    CVertex* v_a = PositionVec (p_a_s, p_a_e);
    CVertex* v_b = PositionVec (p_b_s, p_b_e);

    float result = v_a->GetX () * v_b->GetY () - v_a->GetY () * v_b->GetX ();
    v_a->FreeVertex ();
    v_b->FreeVertex ();
    return result;
}

void CMath::Normal (CVertex* p_a_s, CVertex* p_a_e, CVertex* p_b_s, CVertex* p_b_e, float depth, bool reverse, GLfloat normal[])
{
    float a_x = 0.0;
    float a_y = 0.0;
    float a_z = depth;
    float a_size = sqrtf (powf (a_x, 2) + powf (a_y, 2) + powf (a_z, 2));
    float b_x = p_b_e->GetX () - p_b_s->GetX ();
    float b_y = p_b_e->GetY () - p_b_s->GetY ();
    float b_z = 0.0;
    float b_size = sqrtf (powf (b_x, 2) + powf (b_y, 2) + powf (b_z, 2));

    float c_x = a_y * b_z - a_z * b_y;
    float c_y = a_z * b_x - a_x * b_z;
    float c_z = a_x * b_y - a_y * b_x;
    float c_size = a_size * b_size;

    GLfloat normal_x = c_x / c_size;
    GLfloat normal_y = c_y / c_size;
    GLfloat normal_z = c_z / c_size;

    if (reverse)
    {
        normal[0] = -normal_x;
        normal[1] = -normal_y;
        normal[2] = -normal_z;
    }
    else
    {
        normal[0] = normal_x;
        normal[1] = normal_y;
        normal[2] = normal_z;
    }
}

CVertex* CMath::PositionVec (CVertex* p_s, CVertex* p_e)
{
    CVertex* new_point_vector = new CVertex;
    new_point_vector->SetXY (p_e->GetX () - p_s->GetX (), p_e->GetY () - p_s->GetY ());
    return new_point_vector;
}

float CMath::VecAngle (CVertex* p_a_s, CVertex* p_a_e, CVertex* p_b_s, CVertex* p_b_e)
{
    float outer_product = Outer2DSize (p_a_s, p_a_e, p_b_s, p_b_e);
    float inner_product = Inner (p_a_s, p_a_e, p_b_s, p_b_e);
    return atan2f (outer_product, inner_product);
}
