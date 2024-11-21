#include "pch.h"
#include "CAdminControl.h"
#include "CShape.h"
#include "CMath.h"

CAdminControl::CAdminControl ()
{
    shape_head = NULL;
    shape_tail = NULL;
    shape_num = 0;
}

CAdminControl::~CAdminControl ()
{
    shape_head->FreeShape ();
}

void CAdminControl::Draw (float new_x, float new_y)
{
    CVertex mouse (new_x, new_y, NULL, NULL);

    // 座標軸を表示する
    if (IsShowingAxis ())
    {
        DrawAxis ();
    }

    // 予測点を表示する．
    DrawExVertex (&mouse);

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
            DrawShape (sp);
        }

        // 予測線を表示する．
        if (shape_tail->GetVertexNum () > 0 && IsEditMode ())
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

void CAdminControl::DrawShape (CShape* shape)
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

void CAdminControl::DrawExVertex (CVertex* mouse)
{
    if (EditModeFlag && shape_num > 0 && shape_tail->GetVertexNum () >= 3 && CMath::VertexDis (shape_tail->GetHead (), mouse) < 0.1 && !shape_tail->IsNewVertexSelfCross (mouse) && !IsNewVertexOtherCross (mouse))
    {
        mouse->SetXY (shape_tail->GetHead ()->GetX (), shape_tail->GetHead ()->GetY ());
    }
    else if (EditModeFlag && shape_num > 0 && shape_tail->GetVertexNum () > 0 && shape_tail->IsNewVertexSelfCross (mouse))
    {
        glColor3f (COLOR_RED);
    }
    else if (EditModeFlag && shape_num > 0 && shape_tail->GetVertexNum () > 0 && IsNewVertexOtherCross (mouse))
    {
        glColor3f (COLOR_RED);
    }
    else if (EditModeFlag && shape_num > 0 && shape_tail->GetVertexNum () > 0 && CMath::VertexDis (shape_tail->GetHead (), mouse) < 0.1 && IsNewShapeContaining ())
    {
        glColor3f (COLOR_RED);
    }
    else if (EditModeFlag && shape_num > 0 && IsNewVertexContained (mouse))
    {
        glColor3f (COLOR_RED);
    }
    else
    {
        glColor3f (COLOR_BLACK);
    }
    glPointSize (POINTSIZE);
    glBegin (GL_POINTS);
    glVertex2f (mouse->GetX (), mouse->GetY ());
    glEnd ();
}

void CAdminControl::DrawExLine (CVertex* start, CVertex* end)
{
    CVertex mouse (end->GetX (), end->GetY (), NULL, NULL);

    glEnable (GL_LINE_STIPPLE);
    glLineStipple (2, 0xF0F0);
    if (shape_tail->IsNewVertexSelfCross (&mouse))
    {
        glColor3f (COLOR_RED);
    }
    else if (IsNewVertexOtherCross (&mouse))
    {
        glColor3f (COLOR_RED);
    }
    else if (IsNewShapeContaining () && CMath::VertexDis (shape_tail->GetHead (), &mouse) < 0.1)
    {
        glColor3f (COLOR_RED);
    }
    else
    {
        glColor3f (COLOR_BLACK);
    }
    glBegin (GL_LINES);
    glVertex2f (start->GetX (), start->GetY ());
    glVertex2f (end->GetX (), end->GetY ());
    glEnd ();
    glDisable (GL_LINE_STIPPLE);
}

void CAdminControl::SelectShapeElements (float mouse_x, float mouse_y, UINT nFlags)
{
    CVertex mouse (mouse_x, mouse_y, NULL, NULL);
    if (shape_num > 0 && EditModeFlag && (nFlags & MK_LBUTTON))
    {
        DeSelectAllShape ();
        SelectNearestLine (&mouse);
    }
    else if (shape_num > 0 && EditModeFlag && (nFlags & MK_RBUTTON))
    {
        DeSelectAllShape ();
        SelectNearestVertex (&mouse);
    }
    else if (shape_num > 0 && !EditModeFlag)
    {
        // Ctrl を押しながら左クリックで複数選択
        if (!(nFlags & MK_CONTROL))
        {
            DeSelectAllShape ();
        }
        SelectNearestVertex (&mouse);
        SelectNearestLine (&mouse);
        SelectNearestShape (&mouse);
    }
}

CVertex* CAdminControl::SelectNearestVertex (CVertex* mouse)
{
    for (CShape* sp = shape_head; sp != NULL; sp = sp->GetNext ())
    {
        for (CVertex* vp = sp->GetHead (); vp != NULL; vp = vp->GetNext ())
        {
            if (CMath::VertexDis (vp, mouse) < 0.1 && SelectNearestShape (mouse) == NULL)
            {
                vp->Select ();
                vp->SetLastXY (vp->GetX (), vp->GetY ());

                if (EditModeFlag && sp->GetVertexNum () > 3 && !sp->IsRemoveVertexSelfCross (vp) && !IsRemoveVertexOtherCross (sp, vp))
                {
                    sp->RemoveVertex (vp);
                }
                return vp;
            }
        }
    }

    return NULL;
}

CVertex* CAdminControl::SelectNearestLine (CVertex* mouse)
{
    for (CShape* sp = shape_head; sp != NULL && sp->GetVertexNum () > 0; sp = sp->GetNext ())
    {
        for (CVertex* vp = sp->GetHead (); vp != sp->GetTail (); vp = vp->GetNext ())
        {
            if (CMath::LineDis (mouse, vp, vp->GetNext ()) < 0.1 && SelectNearestVertex (mouse) == NULL && SelectNearestShape (mouse) == NULL)
            {
                vp->Select ();
                vp->SetLastXY (vp->GetX (), vp->GetY ());
                vp->GetNext ()->Select ();
                vp->GetNext ()->SetLastXY (vp->GetNext ()->GetX (), vp->GetNext ()->GetY ());

                if (EditModeFlag)
                {
                    sp->InsertVertex (vp, mouse->GetX (), mouse->GetY (), vp->GetNext ());
                }
                return vp;
            }
        }
        if (CMath::LineDis (mouse, sp->GetTail (), sp->GetHead ()) < 0.1 && SelectNearestVertex (mouse) == NULL && SelectNearestShape (mouse) == NULL)
        {
            sp->GetHead ()->Select ();
            sp->GetHead ()->SetLastXY (sp->GetHead ()->GetX (), sp->GetHead ()->GetY ());
            sp->GetTail ()->Select ();
            sp->GetTail ()->SetLastXY (sp->GetTail ()->GetX (), sp->GetTail ()->GetY ());

            if (EditModeFlag)
            {
                sp->PushVertex (mouse->GetX (), mouse->GetY ());
            }
            return sp->GetTail ();
        }
    }

    return NULL;
}

CShape* CAdminControl::SelectNearestShape (CVertex* mouse)
{
    for (CShape* sp = shape_head; sp != shape_tail; sp = sp->GetNext ())
    {
        if (CMath::IsContained (sp, mouse))
        {
            sp->Select ();
            return sp;
        }
    }

    return NULL;
}

void CAdminControl::TrackVertexToMouse (float mouse_x, float mouse_y)
{
    for (CShape* sp = shape_head; sp != NULL; sp = sp->GetNext ())
    {
        for (CVertex* vp = sp->GetHead (); vp != NULL; vp = vp->GetNext ())
        {
            if (vp->IsSelected ())
            {
                vp->SetXY (mouse_x, mouse_y);
            }
        }
    }
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

void CAdminControl::AddVertex (float new_x, float new_y)
{
    CVertex new_vertex (new_x, new_y, NULL, NULL);

    if (shape_num == 0)
    {
        PushShape ();
    }

    if (shape_num >= 2)
    {
        if (IsNewVertexContained (&new_vertex) || IsNewVertexOtherCross (&new_vertex))
        {
            return;
        }
    }

    if (shape_tail->GetVertexNum () < 3)
    {
        shape_tail->PushVertex (new_x, new_y);
    }
    else if (CMath::VertexDis (shape_tail->GetHead (), &new_vertex) < 0.1 && !shape_tail->IsNewVertexSelfCross (&new_vertex) && !IsNewShapeContaining ())
    {
        shape_tail->Close ();
        PushShape ();
    }
    else if (CMath::VertexDis (shape_tail->GetHead (), &new_vertex) < 0.1 && IsNewShapeContaining ())
    {
        return;
    }
    else
    {
        shape_tail->PushVertex (new_x, new_y);
    }
}

void CAdminControl::SubVertex ()
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

void CAdminControl::SwitchEditMode ()
{
    EditModeFlag = !EditModeFlag;
    DeSelectAllShape ();
}

bool CAdminControl::IsEditMode ()
{
    return EditModeFlag;
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

bool CAdminControl::IsInvalidMovedVertex ()
{
    for (CShape* sp = shape_head; sp != NULL; sp = sp->GetNext ())
    {
        for (CVertex* vp = sp->GetHead (); vp != NULL; vp = vp->GetNext ())
        {
            //
            // 【削除】単純にマウスカーソルに追従するときに2点が重なる問題を一時的に解決
            //
            if (sp->GetVertexNum () == 2)
            {
                if (CMath::VertexDis (sp->GetHead (), sp->GetTail ()) == 0)
                {
                    return true;
                }
            }
            else if (sp->GetVertexNum () == 3)
            {
                if (CMath::VertexDis (sp->GetHead (), sp->GetHead ()->GetNext ()) == 0)
                {
                    return true;
                }
                else if (CMath::VertexDis (sp->GetTail (), sp->GetTail ()->GetPre ()) == 0)
                {
                    return true;
                }
                else if (CMath::VertexDis (sp->GetHead (), sp->GetTail ()) == 0)
                {
                    return true;
                }
            }
            else
            {
                if (CMath::VertexDis (sp->GetHead (), sp->GetHead ()->GetNext ()) == 0)
                {
                    return true;
                }
                else if (CMath::VertexDis (sp->GetTail (), sp->GetTail ()->GetPre ()) == 0)
                {
                    return true;
                }
            }
            //
            //【削除】
            //

            if (vp->IsSelected () && sp->IsMovedVertexSelfCross (vp))
            {
                return true;
            }
            if (vp->IsSelected () && IsMovedVertexContained (sp, vp))
            {
                return true;
            }
            if (vp->IsSelected () && IsMovedShapeContaining (sp))
            {
                return true;
            }
            if (vp->IsSelected () && IsMovedVertexOtherCross (sp, vp))
            {
                return true;
            }
        }
    }
    return false;
}

bool CAdminControl::IsMovedVertexOtherCross (CShape* my_shape, CVertex* moved_vertex)
{
    if (my_shape->GetVertexNum () > 0)
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
            if (moved_vertex == my_shape->GetHead ())
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
            else if (moved_vertex == my_shape->GetTail ())
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
            else
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

bool CAdminControl::IsRemoveVertexOtherCross (CShape* my_shape, CVertex* remove_vertex)
{
    CVertex* pre_vertex = new CVertex;
    CVertex* next_vertex = new CVertex;

    if (remove_vertex == my_shape->GetHead ())
    {
        pre_vertex = my_shape->GetTail ();
        next_vertex = remove_vertex->GetNext ();
    }
    else if (remove_vertex == my_shape->GetTail ())
    {
        pre_vertex = remove_vertex->GetPre ();
        next_vertex = my_shape->GetHead ();
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
}
