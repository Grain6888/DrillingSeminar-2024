#include "pch.h"
#include "CAdminControl.h"
#include "CShape.h"
#include "CMath.h"

CAdminControl::CAdminControl ()
{
    shape_head = NULL;
    shape_tail = NULL;
    shape_num = 0;
    bounding_box = NULL;
}

CAdminControl::~CAdminControl ()
{
    shape_head->FreeShape ();
    DestroyBoundingBox ();
}

void CAdminControl::Draw (float mouse_x, float mouse_y)
{
    CVertex mouse (mouse_x, mouse_y, NULL, NULL);

    // 座標軸を表示する
    if (IsShowingAxis ())
    {
        DrawAxis ();
    }

    if (shape_num > 0)
    {
        for (CShape* sp = shape_head; sp != NULL; sp = sp->GetNext ())
        {
            // 頂点を描画する
            for (CVertex* vp = sp->GetHead (); vp != NULL; vp = vp->GetNext ())
            {
                DrawVertex (vp);
            }

            // 輪郭線を描画する．
            DrawOutline (sp);
        }

        // バウンディングボックスを描画する．
        if (IsScaleMode ())
        {
            DrawScalingGuide (&mouse);
        }
        else if (IsRotateMode ())
        {
            DrawRotatingGuide (&mouse);
        }

        // 予測線を表示する．
        if (IsFreeShapeMode () && shape_tail->GetVertexNum () > 0)
        {
            DrawExLine (shape_tail->GetTail (), &mouse);
        }
    }
}

void CAdminControl::DrawVertex (CVertex* vertex)
{
    if (vertex->IsSelected ())
    {
        glColor3f (COLOR_GREEN);
    }
    else
    {
        glColor3f (COLOR_BLACK);
    }
    glPointSize (POINTSIZE);
    glBegin (GL_POINTS);
    glVertex2f (vertex->GetX (), vertex->GetY ());
    glEnd ();
}

void CAdminControl::DrawLine (CVertex* start, CVertex* end)
{
    if (start->IsSelected () && end->IsSelected ())
    {
        glColor3f (COLOR_GREEN);
    }
    else
    {
        glColor3f (COLOR_BLACK);
    }
    glLineWidth (LINEWIDTH);
    glBegin (GL_LINES);
    glVertex2f (start->GetX (), start->GetY ());
    glVertex2f (end->GetX (), end->GetY ());
    glEnd ();
}

void CAdminControl::DrawOutline (CShape* shape)
{
    for (CVertex* vp = shape->GetHead (); vp != shape->GetTail (); vp = vp->GetNext ())
    {
        DrawLine (vp, vp->GetNext ());
    }

    if (shape != shape_tail)
    {
        DrawLine (shape->GetHead (), shape->GetTail ());
    }
}

void CAdminControl::DrawExLine (CVertex* start, CVertex* end)
{
    CVertex mouse (end->GetX (), end->GetY (), NULL, NULL);

    if (FreeShapeModeFlag)
    {
        glEnable (GL_LINE_STIPPLE);
        glLineStipple (2, 0xF0F0);
        if (CanAddVertex (end))
        {
            glColor3f (COLOR_BLACK);
        }
        else
        {
            glColor3f (COLOR_RED);
        }
        glBegin (GL_LINES);
        glVertex2f (start->GetX (), start->GetY ());
        glVertex2f (end->GetX (), end->GetY ());
        glEnd ();
        glDisable (GL_LINE_STIPPLE);
    }
}

void CAdminControl::DrawScalingGuide (CVertex* base_p)
{
    if (bounding_box != NULL)
    {
        glColor3f (COLOR_ORANGE);
        glPointSize (POINTSIZE);
        glBegin (GL_POINTS);
        for (CVertex* vp = bounding_box->GetHead (); vp != NULL; vp = vp->GetNext ())
        {
            glVertex2f (vp->GetX (), vp->GetY ());
        }
        glVertex2f (base_p->GetX (), base_p->GetY ()); // 基点
        glEnd ();

        glEnable (GL_LINE_STIPPLE);
        glLineStipple (2, 0xF0F0);
        glColor3f (COLOR_ORANGE);
        glBegin (GL_LINE_LOOP);
        for (CVertex* vp = bounding_box->GetHead (); vp != NULL; vp = vp->GetNext ())
        {
            glVertex2f (vp->GetX (), vp->GetY ());
        }
        glEnd ();
        glDisable (GL_LINE_STIPPLE);
    }
}

void CAdminControl::DrawRotatingGuide (CVertex* base_p)
{
    if (bounding_box != NULL)
    {
        glColor3f (COLOR_BLUE);
        glPointSize (POINTSIZE);
        glBegin (GL_POINTS);
        for (CVertex* vp = bounding_box->GetHead (); vp != NULL; vp = vp->GetNext ())
        {
            glVertex2f (vp->GetX (), vp->GetY ());
        }
        glVertex2f (base_p->GetX (), base_p->GetY ()); // 基点
        glEnd ();

        glEnable (GL_LINE_STIPPLE);
        glLineStipple (2, 0xF0F0);
        glColor3f (COLOR_BLUE);
        glBegin (GL_LINE_LOOP);
        for (CVertex* vp = bounding_box->GetHead (); vp != NULL; vp = vp->GetNext ())
        {
            glVertex2f (vp->GetX (), vp->GetY ());
        }
        glEnd ();
        glDisable (GL_LINE_STIPPLE);
    }
}

CVertex* CAdminControl::SelectVertex (CVertex* mouse)
{
    for (CShape* sp = shape_head; sp != NULL; sp = sp->GetNext ())
    {
        for (CVertex* vp = sp->GetHead (); vp != NULL; vp = vp->GetNext ())
        {
            if (CMath::VertexDis (vp, mouse) < MIN_DISTANCE)
            {
                vp->Select ();
                vp->SetLastXY (vp->GetX (), vp->GetY ());
                return vp;
            }
        }
    }

    return NULL;
}

CVertex* CAdminControl::SelectLine (CVertex* mouse)
{
    for (CShape* sp = shape_head; sp != NULL && sp->GetVertexNum () > 0; sp = sp->GetNext ())
    {
        for (CVertex* vp = sp->GetHead (); vp != sp->GetTail (); vp = vp->GetNext ())
        {
            if (CMath::LineDis (mouse, vp, vp->GetNext ()) < MIN_DISTANCE)
            {
                vp->Select ();
                vp->SetLastXY (vp->GetX (), vp->GetY ());
                vp->GetNext ()->Select ();
                vp->GetNext ()->SetLastXY (vp->GetNext ()->GetX (), vp->GetNext ()->GetY ());
                return vp;
            }
        }
        if (sp->IsClosed () == true && CMath::LineDis (mouse, sp->GetTail (), sp->GetHead ()) < MIN_DISTANCE)
        {
            sp->GetHead ()->Select ();
            sp->GetHead ()->SetLastXY (sp->GetHead ()->GetX (), sp->GetHead ()->GetY ());
            sp->GetTail ()->Select ();
            sp->GetTail ()->SetLastXY (sp->GetTail ()->GetX (), sp->GetTail ()->GetY ());
            return sp->GetTail ();
        }
    }

    return NULL;
}

CShape* CAdminControl::SelectShape (CVertex* mouse)
{
    for (CShape* sp = shape_head; sp != shape_tail; sp = sp->GetNext ())
    {
        if (CMath::IsContained (sp, mouse))
        {
            sp->Select ();
            CreateBoundingBox ();
            UpdateBoundingBox ();
            return sp;
        }
    }

    return NULL;
}

void CAdminControl::ShiftVertex (float mouse_before_x, float mouse_before_y, float mouse_after_x, float mouse_after_y)
{
    CVertex before (mouse_before_x, mouse_before_y, NULL, NULL);
    CVertex after (mouse_after_x, mouse_after_y, NULL, NULL);

    for (CShape* sp = shape_head; sp != NULL && sp->IsClosed () == true; sp = sp->GetNext ())
    {
        for (CVertex* vp = sp->GetHead (); vp != NULL; vp = vp->GetNext ())
        {
            if (vp->IsSelected ())
            {
                CMath::ShiftPoint (&before, &after, vp);
            }
        }
    }

    UpdateBoundingBox ();
}

void CAdminControl::ScaleShape (float mouse_before_x, float mouse_before_y, float mouse_after_x, float mouse_after_y)
{
    CVertex before (mouse_before_x, mouse_before_y, NULL, NULL);
    CVertex after (mouse_after_x, mouse_after_y, NULL, NULL);

    for (CShape* sp = shape_head; sp != NULL && sp->IsClosed () == true; sp = sp->GetNext ())
    {
        if (sp->IsSelected ())
        {
            for (CVertex* vp = sp->GetHead (); vp != NULL; vp = vp->GetNext ())
            {
                CMath::ScalePoint (&before, &after, vp);
            }
        }
    }

    UpdateBoundingBox ();
}

void CAdminControl::ScaleUpShape (CVertex* base_p)
{
    for (CShape* sp = shape_head; sp != shape_tail; sp = sp->GetNext ())
    {
        if (sp->IsSelected ())
        {
            for (CVertex* vp = sp->GetHead (); vp != NULL; vp = vp->GetNext ())
            {
                CMath::ScalePoint (1.05f, base_p, vp);
            }
        }
    }

    UpdateBoundingBox ();
}

void CAdminControl::ScaleDownShape (CVertex* base_p)
{
    for (CShape* sp = shape_head; sp != shape_tail; sp = sp->GetNext ())
    {
        if (sp->IsSelected ())
        {
            for (CVertex* vp = sp->GetHead (); vp != NULL; vp = vp->GetNext ())
            {
                CMath::ScalePoint (0.95f, base_p, vp);
            }
        }
    }

    UpdateBoundingBox ();
}

void CAdminControl::RotateLeftShape (CVertex* base_p)
{
    for (CShape* sp = shape_head; sp != shape_tail; sp = sp->GetNext ())
    {
        if (sp->IsSelected ())
        {
            for (CVertex* vp = sp->GetHead (); vp != NULL; vp = vp->GetNext ())
            {
                CMath::RotatePoint (10, base_p, vp);
            }
        }
    }

    UpdateBoundingBox ();
}

void CAdminControl::RotateRightShape (CVertex* base_p)
{
    for (CShape* sp = shape_head; sp != shape_tail; sp = sp->GetNext ())
    {
        if (sp->IsSelected ())
        {
            for (CVertex* vp = sp->GetHead (); vp != NULL; vp = vp->GetNext ())
            {
                CMath::RotatePoint (-10, base_p, vp);
            }
        }
    }

    UpdateBoundingBox ();
}

void CAdminControl::ResetMovedVertex ()
{
    for (CShape* sp = shape_head; sp != NULL; sp = sp->GetNext ())
    {
        for (CVertex* vp = sp->GetHead (); vp != NULL; vp = vp->GetNext ())
        {
            if (vp->IsSelected ())
            {
                vp->SetXY (vp->GetLastX (), vp->GetLastY ());
            }
        }
    }

    UpdateBoundingBox ();
}

void CAdminControl::UpdateLastMovedVertex ()
{
    for (CShape* sp = shape_head; sp != shape_tail; sp = sp->GetNext ())
    {
        for (CVertex* vp = sp->GetHead (); vp != NULL; vp = vp->GetNext ())
        {
            if (vp->IsSelected ())
            {
                vp->SetLastXY (vp->GetX (), vp->GetY ());
            }
        }
    }
}

void CAdminControl::PushShape ()
{
    CShape* new_s = new CShape;
    CShape* pre_s = shape_tail;

    if (shape_num == 0)
    {
        shape_head = new_s;
    }
    else
    {
        shape_tail->SetNext (new_s);
        new_s->SetPre (pre_s);
    }
    shape_tail = new_s;
    shape_num++;
}

void CAdminControl::PopShape ()
{
    if (shape_num == 0)
    {
        return;
    }
    else if (shape_num == 1)
    {
        shape_head->FreeShape ();
        shape_head = NULL;
        shape_tail = NULL;
        shape_num--;
    }
    else
    {
        CShape* pre_sp = shape_tail->GetPre ();
        pre_sp->SetNext (NULL);
        shape_tail->FreeShape ();
        shape_tail = pre_sp;
        shape_num--;
    }
}

void CAdminControl::RemoveShape ()
{
    CShape* pre_shape;
    CShape* next_shape;

    if (shape_head->IsSelected ())
    {
        next_shape = shape_head->GetNext ();
        next_shape->SetPre (NULL);
        shape_head->SetNext (NULL);
        shape_head->FreeShape ();
        shape_head = next_shape;
    }
    else
    {
        for (CShape* sp = shape_head->GetNext (); sp != shape_tail; sp = sp->GetNext ())
        {
            if (sp->IsSelected ())
            {
                pre_shape = sp->GetPre ();
                next_shape = sp->GetNext ();
                pre_shape->SetNext (next_shape);
                next_shape->SetPre (pre_shape);
                sp->SetPre (NULL);
                sp->SetNext (NULL);
                sp->FreeShape ();
                break;
            }
        }
    }

    DestroyBoundingBox ();

    shape_num--;
}

void CAdminControl::DeleteAllShape ()
{
    if (shape_num > 0)
    {
        shape_head->FreeShape ();
        shape_head = NULL;
        shape_tail = NULL;
        shape_num = 0;
    }
}

int CAdminControl::GetShapeNum ()
{
    return shape_num;
}

void CAdminControl::PushVertex (float new_x, float new_y)
{
    CVertex new_vertex (new_x, new_y, NULL, NULL);

    if (shape_num == 0)
    {
        PushShape ();
    }

    if (CanAddVertex (&new_vertex))
    {
        if (shape_tail->GetVertexNum () >= 3 && CMath::VertexDis (shape_tail->GetHead (), &new_vertex) < MIN_DISTANCE)
        {
            shape_tail->Close ();
            PushShape ();
        }
        else
        {
            shape_tail->PushVertex (new_x, new_y);
        }
    }
}

void CAdminControl::PopVertex ()
{
    if (shape_num == 0)
    {
        return;
    }
    else if (shape_tail->GetVertexNum () == 0)
    {
        PopShape ();
    }
    else
    {
        shape_tail->Open ();
        shape_tail->PopVertex ();
    }
}

void CAdminControl::AddVertex (float mouse_x, float mouse_y)
{
    CVertex mouse (mouse_x, mouse_y, NULL, NULL);

    for (CShape* sp = shape_head; sp != shape_tail; sp = sp->GetNext ())
    {
        if (sp->GetHead ()->IsSelected () && sp->GetTail ()->IsSelected ())
        {
            CVertex new_vertex;
            CMath::CrossPoint (&mouse, sp->GetTail (), sp->GetHead (), &new_vertex);
            sp->PushVertex (new_vertex.GetX (), new_vertex.GetY ());
            return;
        }

        for (CVertex* vp = sp->GetHead (); vp != sp->GetTail (); vp = vp->GetNext ())
        {
            if (vp->IsSelected ())
            {
                CVertex new_vertex;
                CMath::CrossPoint (&mouse, vp, vp->GetNext (), &new_vertex);
                sp->InsertVertex (vp, new_vertex.GetX (), new_vertex.GetY (), vp->GetNext ());
                return;
            }
        }
    }
}

void CAdminControl::SubVertex ()
{
    for (CShape* sp = shape_head; sp != shape_tail; sp = sp->GetNext ())
    {
        if (sp->GetTail ()->IsSelected () && sp->GetVertexNum () > 3 && CanRemoveVertex ())
        {
            sp->PopVertex ();
        }
        for (CVertex* vp = sp->GetHead (); vp != NULL; vp = vp->GetNext ())
        {
            if (vp->IsSelected () && sp->GetVertexNum () > 3 && CanRemoveVertex ())
            {
                sp->RemoveVertex (vp);
                return;
            }
        }
    }
}

void CAdminControl::DrawSizeUp ()
{
    if (LINEWIDTH <= 10.0)
    {
        POINTSIZE += 0.5f;
        LINEWIDTH += 0.5f;
    }
}

void CAdminControl::DrawSizeDown ()
{
    if (LINEWIDTH >= 1.0)
    {
        POINTSIZE -= 0.5f;
        LINEWIDTH -= 0.5f;
    }
}

void CAdminControl::DrawAxis ()
{
    glBegin (GL_LINES);
    // x軸
    glColor3f (1.0, 0.0, 0.0);
    glVertex3f (-1.0, 0.0, 0.0);
    glVertex3f (1.0, 0.0, 0.0);
    // y軸
    glColor3f (0.0, 1.0, 0.0);
    glVertex3f (0.0, -1.0, 0.0);
    glVertex3f (0.0, 1.0, 0.0);
    // z軸
    glColor3f (0.0, 0.0, 1.0);
    glVertex3f (0.0, 0.0, -1.0);
    glVertex3f (0.0, 0.0, 1.0);
    glEnd ();
}

void CAdminControl::SwitchAxis ()
{
    AxisFlag = !AxisFlag;
}

bool CAdminControl::IsShowingAxis ()
{
    return AxisFlag;
}

void CAdminControl::SwitchFreeShapeMode ()
{
    FreeShapeModeFlag = !FreeShapeModeFlag;
    DeSelectAllShape ();

    ClearAffineTransMode ();
}

bool CAdminControl::IsFreeShapeMode ()
{
    return FreeShapeModeFlag;
}

void CAdminControl::SwitchAffineTransMode ()
{
    if (!ScalingModeFlag && !RotatingModeFlag)
    {
        ScalingModeFlag = true;
        RotatingModeFlag = false;
    }
    else if (ScalingModeFlag)
    {
        ScalingModeFlag = false;
        RotatingModeFlag = true;
    }
    else if (RotatingModeFlag)
    {
        ScalingModeFlag = true;
        RotatingModeFlag = false;
    }
}

void CAdminControl::ClearAffineTransMode ()
{
    ScalingModeFlag = false;
    RotatingModeFlag = false;
}

bool CAdminControl::IsScaleMode ()
{
    return ScalingModeFlag;
}

bool CAdminControl::IsRotateMode ()
{
    return RotatingModeFlag;
}

bool CAdminControl::CanAddVertex (CVertex* new_vertex)
{
    if (shape_num == 1)
    {
        if (shape_tail->GetVertexNum () >= 3 && CMath::VertexDis (shape_tail->GetHead (), new_vertex) < MIN_DISTANCE)
        {
            new_vertex->SetXY (shape_tail->GetHead ()->GetX (), shape_tail->GetHead ()->GetY ());
        }
    }
    if (shape_num >= 1)
    {
        if (shape_tail->IsNewVertexSelfCross (new_vertex))
        {
            return false;
        }
    }
    if (shape_num >= 2)
    {
        if (IsNewVertexContained (new_vertex) || IsNewVertexOtherCross (new_vertex))
        {
            return false;
        }
        if (shape_tail->GetVertexNum () >= 3 && CMath::VertexDis (shape_tail->GetHead (), new_vertex) < MIN_DISTANCE)
        {
            if (IsNewShapeContaining ())
            {
                return false;
            }
            else
            {
                new_vertex->SetXY (shape_tail->GetHead ()->GetX (), shape_tail->GetHead ()->GetY ());
            }
        }
    }

    return true;
}

bool CAdminControl::IsNewVertexContained (CVertex* new_vertex)
{
    for (CShape* sp = shape_head; sp != shape_tail; sp = sp->GetNext ())
    {
        if (CMath::IsContained (sp, new_vertex))
        {
            return true;
        }
    }

    return false;
}

bool CAdminControl::IsNewShapeContaining ()
{
    for (CShape* sp = shape_head; sp != shape_tail; sp = sp->GetNext ())
    {
        for (CVertex* vp = sp->GetHead (); vp != NULL; vp = vp->GetNext ())
        {
            if (CMath::IsContained (shape_tail, vp))
            {
                return true;
            }
        }
    }

    return false;
}

bool CAdminControl::IsNewVertexOtherCross (CVertex* new_vertex)
{
    if (shape_tail->GetVertexNum () > 0)
    {
        for (CShape* sp = shape_head; sp != shape_tail; sp = sp->GetNext ())
        {
            // 図形の始点から終点までに存在する辺を対象に，他交差判定を行う．
            for (CVertex* vp = sp->GetHead (); vp != sp->GetTail (); vp = vp->GetNext ())
            {
                if (CMath::IsLineCrossing (vp, vp->GetNext (), shape_tail->GetTail (), new_vertex))
                {
                    return true;
                }
            }
            // 図形の終点から始点に伸びる辺を対象に，他交差判定を行う．
            if (CMath::IsLineCrossing (sp->GetHead (), sp->GetTail (), shape_tail->GetTail (), new_vertex))
            {
                return true;
            }
        }
    }

    return false;
}

bool CAdminControl::CanMoveVertex ()
{
    for (CShape* sp = shape_head; sp != NULL; sp = sp->GetNext ())
    {
        for (CVertex* vp = sp->GetHead (); vp != NULL; vp = vp->GetNext ())
        {
            if (vp->IsSelected () && sp->IsMovedVertexSelfCross (vp))
            {
                return false;
            }
            if (vp->IsSelected () && IsMovedVertexContained (sp, vp))
            {
                return false;
            }
            if (vp->IsSelected () && IsMovedShapeContaining (sp))
            {
                return false;
            }
            if (vp->IsSelected () && IsMovedVertexOtherCross (sp, vp))
            {
                return false;
            }
        }
    }
    return true;
}

bool CAdminControl::IsMovedVertexOtherCross (CShape* my_shape, CVertex* moved_vertex)
{
    if (my_shape->GetVertexNum () > 1)
    {
        for (CShape* sp = shape_head; sp != NULL && sp->GetVertexNum () > 0; sp = sp->GetNext ())
        {
            if (sp == my_shape)
            {
                continue;
            }

            for (CVertex* vp = sp->GetHead (); vp != sp->GetTail (); vp = vp->GetNext ())
            {
                if (moved_vertex == my_shape->GetHead ())
                {
                    if (CMath::IsLineCrossing (vp, vp->GetNext (), moved_vertex, moved_vertex->GetNext ()))
                    {
                        return true;
                    }
                    else if (CMath::IsLineCrossing (vp, vp->GetNext (), moved_vertex, my_shape->GetTail ()))
                    {
                        return true;
                    }
                }
                else if (moved_vertex == my_shape->GetTail ())
                {
                    if (CMath::IsLineCrossing (vp, vp->GetNext (), moved_vertex, my_shape->GetHead ()))
                    {
                        return true;
                    }
                    else if (CMath::IsLineCrossing (vp, vp->GetNext (), moved_vertex, moved_vertex->GetPre ()))
                    {
                        return true;
                    }
                }
                else
                {
                    if (CMath::IsLineCrossing (vp, vp->GetNext (), moved_vertex, moved_vertex->GetNext ()))
                    {
                        return true;
                    }
                    else if (CMath::IsLineCrossing (vp, vp->GetNext (), moved_vertex, moved_vertex->GetPre ()))
                    {
                        return true;
                    }
                }
            }
            if (sp->IsClosed () == true && moved_vertex == my_shape->GetHead ())
            {
                if (CMath::IsLineCrossing (sp->GetHead (), sp->GetTail (), moved_vertex, moved_vertex->GetNext ()))
                {
                    return true;
                }
                else if (CMath::IsLineCrossing (sp->GetHead (), sp->GetTail (), moved_vertex, my_shape->GetTail ()))
                {
                    return true;
                }
            }
            else if (sp->IsClosed () == true && moved_vertex == my_shape->GetTail ())
            {
                if (CMath::IsLineCrossing (sp->GetHead (), sp->GetTail (), moved_vertex, my_shape->GetHead ()))
                {
                    return true;
                }
                else if (CMath::IsLineCrossing (sp->GetHead (), sp->GetTail (), moved_vertex, moved_vertex->GetPre ()))
                {
                    return true;
                }
            }
            else if (sp->IsClosed () == true)
            {
                if (CMath::IsLineCrossing (sp->GetHead (), sp->GetTail (), moved_vertex, moved_vertex->GetNext ()))
                {
                    return true;
                }
                else if (CMath::IsLineCrossing (sp->GetHead (), sp->GetTail (), moved_vertex, moved_vertex->GetPre ()))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool CAdminControl::IsMovedVertexContained (CShape* my_shape, CVertex* moved_vertex)
{
    for (CShape* sp = shape_head; sp != shape_tail; sp = sp->GetNext ())
    {
        if (sp == my_shape)
        {
            continue;
        }
        else if (CMath::IsContained (sp, moved_vertex))
        {
            return true;
        }
    }

    return false;
}

bool CAdminControl::IsMovedShapeContaining (CShape* moved_shape)
{
    if (moved_shape->IsClosed () == false)
    {
        return false;
    }

    for (CShape* sp = shape_head; sp != NULL && sp->GetVertexNum () > 0; sp = sp->GetNext ())
    {
        if (sp == moved_shape)
        {
            continue;
        }
        else if (CMath::IsContained (moved_shape, sp->GetHead ()))
        {
            return true;
        }
    }

    return false;
}

bool CAdminControl::CanRemoveVertex ()
{
    for (CShape* sp = shape_head; sp != NULL; sp = sp->GetNext ())
    {
        for (CVertex* vp = sp->GetHead (); vp != NULL; vp = vp->GetNext ())
        {
            if (vp->IsSelected () && sp->IsRemoveVertexSelfCross (vp))
            {
                return false;
            }
            if (vp->IsSelected () && IsRemoveVertexOtherCross (sp, vp))
            {
                return false;
            }
            if (vp->IsSelected () && IsRemoveShapeContaining (sp, vp))
            {
                return false;
            }
        }
    }
    return true;
}

bool CAdminControl::IsRemoveVertexOtherCross (CShape* my_shape, CVertex* remove_vertex)
{
    CVertex* pre_vertex;
    CVertex* next_vertex;

    if (my_shape->IsClosed () == true && remove_vertex == my_shape->GetHead ())
    {
        pre_vertex = my_shape->GetTail ();
        next_vertex = remove_vertex->GetNext ();
    }
    else if (my_shape->IsClosed () == false && remove_vertex == my_shape->GetHead ())
    {
        return false;
    }
    else if (my_shape->IsClosed () == true && remove_vertex == my_shape->GetTail ())
    {
        pre_vertex = remove_vertex->GetPre ();
        next_vertex = my_shape->GetHead ();
    }
    else if (my_shape->IsClosed () == false && remove_vertex == my_shape->GetTail ())
    {
        return false;
    }
    else
    {
        pre_vertex = remove_vertex->GetPre ();
        next_vertex = remove_vertex->GetNext ();
    }

    for (CShape* sp = shape_head; sp != NULL && sp->GetVertexNum () > 0; sp = sp->GetNext ())
    {
        if (sp == my_shape)
        {
            continue;
        }

        for (CVertex* vp = sp->GetHead (); vp != sp->GetTail (); vp = vp->GetNext ())
        {
            if (vp == pre_vertex)
            {
                if (CMath::IsLineCrossing (vp, vp->GetNext ()->GetNext (), pre_vertex, next_vertex))
                {
                    return true;
                }
                vp = vp->GetNext ();
            }
            else if (CMath::IsLineCrossing (vp, vp->GetNext (), pre_vertex, next_vertex))
            {
                return true;
            }
        }
        if (CMath::IsLineCrossing (sp->GetHead (), sp->GetTail (), pre_vertex, next_vertex))
        {
            return true;
        }
    }

    return false;
}

bool CAdminControl::IsRemoveShapeContaining (CShape* my_shape, CVertex* remove_vertex)
{
    if (my_shape->IsClosed () == false)
    {
        return false;
    }

    for (CShape* sp = shape_head; sp != NULL && sp->GetVertexNum () > 0; sp = sp->GetNext ())
    {
        if (sp == my_shape)
        {
            continue;
        }
        if (CMath::IsContained (my_shape, sp->GetHead (), remove_vertex))
        {
            return true;
        }
    }

    return false;
}

void CAdminControl::SelectAllShape ()
{
    for (CShape* sp = shape_head; sp != NULL; sp = sp->GetNext ())
    {
        sp->Select ();
    }
}

void CAdminControl::DeSelectAllShape ()
{
    for (CShape* sp = shape_head; sp != NULL; sp = sp->GetNext ())
    {
        sp->DeSelect ();
    }
    DestroyBoundingBox ();
}

void CAdminControl::CreateBoundingBox ()
{
    if (bounding_box == NULL)
    {
        CShape* box = new CShape;
        bounding_box = box;

        bool selected_flag = false;
        float max_x = 0.0;
        float min_x = 0.0;
        float max_y = 0.0;
        float min_y = 0.0;

        for (CShape* sp = shape_head; sp != shape_tail; sp = sp->GetNext ())
        {
            if (sp->IsSelected ())
            {
                if (!selected_flag)
                {
                    max_x = sp->GetHead ()->GetX ();
                    min_x = sp->GetHead ()->GetX ();
                    max_y = sp->GetHead ()->GetY ();
                    min_y = sp->GetHead ()->GetY ();
                    selected_flag = true;
                }
                for (CVertex* vp = sp->GetHead (); vp != NULL; vp = vp->GetNext ())
                {
                    if (vp->GetX () > max_x)
                    {
                        max_x = vp->GetX ();
                    }
                    else if (vp->GetX () < min_x)
                    {
                        min_x = vp->GetX ();
                    }

                    if (vp->GetY () > max_y)
                    {
                        max_y = vp->GetY ();
                    }
                    else if (vp->GetY () < min_y)
                    {
                        min_y = vp->GetY ();
                    }
                }
            }
        }

        bounding_box->PushVertex (min_x, max_y); // 左上
        bounding_box->PushVertex (min_x, min_y); // 左下
        bounding_box->PushVertex (max_x, min_y); // 右下
        bounding_box->PushVertex (max_x, max_y); // 右上
    }
}

void CAdminControl::UpdateBoundingBox ()
{
    if (bounding_box != NULL)
    {
        bool selected_flag = false;
        float max_x = 0.0;
        float min_x = 0.0;
        float max_y = 0.0;
        float min_y = 0.0;

        for (CShape* sp = shape_head; sp != shape_tail; sp = sp->GetNext ())
        {
            if (sp->IsSelected ())
            {
                if (!selected_flag)
                {
                    max_x = sp->GetHead ()->GetX ();
                    min_x = sp->GetHead ()->GetX ();
                    max_y = sp->GetHead ()->GetY ();
                    min_y = sp->GetHead ()->GetY ();
                    selected_flag = true;
                }
                for (CVertex* vp = sp->GetHead (); vp != NULL; vp = vp->GetNext ())
                {
                    if (vp->GetX () > max_x)
                    {
                        max_x = vp->GetX ();
                    }
                    else if (vp->GetX () < min_x)
                    {
                        min_x = vp->GetX ();
                    }

                    if (vp->GetY () > max_y)
                    {
                        max_y = vp->GetY ();
                    }
                    else if (vp->GetY () < min_y)
                    {
                        min_y = vp->GetY ();
                    }
                }
            }
        }

        bounding_box->GetHead ()->SetXY (min_x, max_y); // 左上
        bounding_box->GetHead ()->GetNext ()->SetXY (min_x, min_y); // 左下
        bounding_box->GetTail ()->GetPre ()->SetXY (max_x, min_y); // 右下
        bounding_box->GetTail ()->SetXY (max_x, max_y); // 右上
    }
}

void CAdminControl::DestroyBoundingBox ()
{
    if (bounding_box == NULL)
    {
        return;
    }
    else
    {
        bounding_box->FreeShape ();
        bounding_box = NULL;
    }
}
