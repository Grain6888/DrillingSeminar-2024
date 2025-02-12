#include "pch.h"
#include "CMath.h"

float CMath::VertexDis (CVertex* p1, CVertex* p2)
{
    float x_dis = (p2->GetX () - p1->GetX ()) * (p2->GetX () - p1->GetX ());
    float y_dis = (p2->GetY () - p1->GetY ()) * (p2->GetY () - p1->GetY ());
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

    float t = (apx * abx + apy * aby) / (abx * abx + aby * aby);

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

    float x_dis = (px - cx) * (px - cx);
    float y_dis = (py - cy) * (py - cy);
    return sqrtf (x_dis + y_dis);
}

double CMath::TriangleArea (CShape* triangle)
{
    CVertex* v_a = PositionVec (triangle->GetHead (), triangle->GetHead ()->GetNext ());
    CVertex* v_b = PositionVec (triangle->GetHead (), triangle->GetTail ());

    double size_2_v_a = v_a->GetX () * v_a->GetX () + v_a->GetY () * v_a->GetY ();
    double size_2_v_b = v_b->GetX () * v_b->GetX () + v_b->GetY () * v_b->GetY ();
    double inner_a_b = Inner (triangle->GetHead (), triangle->GetHead ()->GetNext (), triangle->GetHead (), triangle->GetTail ());

    double length_a = sqrt (size_2_v_a);
    double length_b = sqrt (size_2_v_b);

    double cos_theta = inner_a_b / (length_a * length_b);
    double sin_theta = sqrt (1 - cos_theta * cos_theta);

    double area = 0.5 * length_a * length_b * sin_theta;

    v_a->FreeVertex ();
    v_b->FreeVertex ();
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

    float t = (apx * abx + apy * aby) / (abx * abx + aby * aby);

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

void CMath::MidPoint (CVertex* line_s, CVertex* line_e, CVertex* result)
{
    GLfloat x = (line_e->GetX () + line_s->GetX ()) / 2;
    GLfloat y = (line_e->GetY () + line_s->GetY ()) / 2;
    result->SetXY (x, y);
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
    double theta = VecAngle (base_p, before, base_p, after);
    GLfloat x = (float)((result->GetLastX () - base_p->GetX ()) * cos (theta) - (result->GetLastY () - base_p->GetY ()) * sin (theta) + base_p->GetX ());
    GLfloat y = (float)((result->GetLastX () - base_p->GetX ()) * sin (theta) + (result->GetLastY () - base_p->GetY ()) * cos (theta) + base_p->GetY ());
    result->SetXY (x, y);
}

void CMath::RotatePoint (float degree, CVertex* base_p, CVertex* result)
{
    double theta = degree / 180 * M_PI;
    GLfloat x = (float)((result->GetLastX () - base_p->GetX ()) * cos (theta) - (result->GetLastY () - base_p->GetY ()) * sin (theta) + base_p->GetX ());
    GLfloat y = (float)((result->GetLastX () - base_p->GetX ()) * sin (theta) + (result->GetLastY () - base_p->GetY ()) * cos (theta) + base_p->GetY ());
    result->SetXY (x, y);
}

bool CMath::IsLineCrossing (CVertex* a_s, CVertex* a_e, CVertex* b_s, CVertex* b_e)
{
    auto is_point_on_segment = [](CVertex* p, CVertex* s, CVertex* e)
        {
            double min_x = (std::min)(s->GetX (), e->GetX ());
            double max_x = (std::max)(s->GetX (), e->GetX ());
            double min_y = (std::min)(s->GetY (), e->GetY ());
            double max_y = (std::max)(s->GetY (), e->GetY ());

            if (fabs (p->GetX () - min_x) > 1e-6 && fabs (p->GetX () - max_x) > 1e-6)
            {
                if (p->GetX () < min_x + 1e-6)
                {
                    return false;
                }
                if (p->GetX () > max_x - 1e-6)
                {
                    return false;
                }
            }

            if (fabs (p->GetY () - min_y) > 1e-6 && fabs (p->GetY () - max_y) > 1e-6)
            {
                if (p->GetY () < min_y + 1e-6)
                {
                    return false;
                }
                if (p->GetY () > max_y - 1e-6)
                {
                    return false;
                }
            }

            return true;
        };

    double outer_a_1 = Outer2DSize (a_s, a_e, a_s, b_s);
    double outer_a_2 = Outer2DSize (a_s, a_e, a_s, b_e);
    double outer_b_1 = Outer2DSize (b_s, b_e, b_s, a_s);
    double outer_b_2 = Outer2DSize (b_s, b_e, b_s, a_e);

    if ((outer_a_1 * outer_a_2 < 0.0) && (outer_b_1 * outer_b_2 < 0.0))
    {
        return true;
    }

    if (fabs (outer_a_1) < 1e-6 && is_point_on_segment (b_s, a_s, a_e))
    {
        return true;
    }
    if (fabs (outer_a_2) < 1e-6 && is_point_on_segment (b_e, a_s, a_e))
    {
        return true;
    }
    if (fabs (outer_b_1) < 1e-6 && is_point_on_segment (a_s, b_s, b_e))
    {
        return true;
    }
    if (fabs (outer_b_2) < 1e-6 && is_point_on_segment (a_e, b_s, b_e))
    {
        return true;
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
    if (fabs (difference) <= 1e-6)
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
    double size_sum = 0.0;
    CVertex p (0.0, 0.0, NULL, NULL);
    for (CVertex* vp = my_shape->GetHead (); vp != my_shape->GetTail (); vp = vp->GetNext ())
    {
        size_sum += CMath::Outer2DSize (&p, vp, &p, vp->GetNext ()) / 2;
    }
    size_sum += CMath::Outer2DSize (&p, my_shape->GetTail (), &p, my_shape->GetHead ()) / 2;

    if (size_sum > 0)
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
    if (fabs (difference) <= 1e-6)
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

double CMath::Outer2DSize (CVertex* p_a_s, CVertex* p_a_e, CVertex* p_b_s, CVertex* p_b_e)
{
    CVertex* v_a = PositionVec (p_a_s, p_a_e);
    CVertex* v_b = PositionVec (p_b_s, p_b_e);

    double result = v_a->GetX () * v_b->GetY () - v_a->GetY () * v_b->GetX ();
    v_a->FreeVertex ();
    v_b->FreeVertex ();
    return result;
}

void CMath::Normal (CVertex* p_a_s, CVertex* p_a_e, CVertex* p_b_s, CVertex* p_b_e, float depth, bool reverse, GLfloat normal[])
{
    float a_x = 0.0;
    float a_y = 0.0;
    float a_z = depth;
    float a_size = sqrtf (a_x * a_x + a_y * a_y + a_z * a_z);
    float b_x = p_b_e->GetX () - p_b_s->GetX ();
    float b_y = p_b_e->GetY () - p_b_s->GetY ();
    float b_z = 0.0;
    float b_size = sqrtf (b_x * b_x + b_y * b_y + b_z * b_z);

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

double CMath::VecAngle (CVertex* p_a_s, CVertex* p_a_e, CVertex* p_b_s, CVertex* p_b_e)
{
    double outer_product = Outer2DSize (p_a_s, p_a_e, p_b_s, p_b_e);
    double inner_product = Inner (p_a_s, p_a_e, p_b_s, p_b_e);
    return atan2 (outer_product, inner_product);
}
