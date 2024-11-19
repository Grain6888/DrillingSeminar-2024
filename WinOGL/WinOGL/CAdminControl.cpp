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

            // 図形を描画する．
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
        glColor3f (0.36f, 0.80f, 0.68f);
    }
    else
    {
        glColor3f (0.15f, 0.15f, 0.15f);
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
        glColor3f (0.36f, 0.80f, 0.68f);
    }
    else
    {
        glColor3f (0.15f, 0.15f, 0.15f);
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
    if (shape_num > 0 && shape_tail->GetVertexNum () >= 3 && CMath::VertexDis (shape_tail->GetHead (), mouse) < 0.1 && !shape_tail->IsNewVertexSelfCross (mouse) && !IsNewVertexOtherCross (mouse) && !IsNewShapeContaining ())
    {
        mouse->SetXY (shape_tail->GetHead ()->GetX (), shape_tail->GetHead ()->GetY ());
    }
    else
    {
        glColor3f (0.15f, 0.15f, 0.15f);
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
    if (shape_tail->IsNewVertexSelfCross (&mouse) || IsNewVertexOtherCross (&mouse) || (IsNewShapeContaining () && CMath::VertexDis (shape_tail->GetHead (), &mouse) < 0.1))
    {
        glColor3f (0.94f, 0.25f, 0.14f);
    }
    else
    {
        glColor3f (0.15f, 0.15f, 0.15f);
    }
    glBegin (GL_LINES);
    glVertex2f (start->GetX (), start->GetY ());
    glVertex2f (end->GetX (), end->GetY ());
    glEnd ();
    glDisable (GL_LINE_STIPPLE);
}

void CAdminControl::SelectShapeElements (float mouse_x, float mouse_y)
{
    CVertex mouse (mouse_x, mouse_y, NULL, NULL);
    if (shape_num > 0)
    {
        DeSelectAllShape ();
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
            if (CMath::VertexDis (vp, mouse) < 0.1)
            {
                vp->Select ();
                vp->SetLastXY (vp->GetX (), vp->GetY ());
                return vp;
            }
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
            DeSelectAllShape ();
            sp->Select ();
            return sp;
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
            if (CMath::LineDis (mouse, vp, vp->GetNext ()) < 0.1 && SelectNearestVertex (mouse) == NULL)
            {
                vp->Select ();
                vp->GetNext ()->Select ();
                return vp;
            }
        }
        if (CMath::LineDis (mouse, sp->GetTail (), sp->GetHead ()) < 0.1 && SelectNearestVertex (mouse) == NULL)
        {
            sp->GetHead ()->Select ();
            sp->GetTail ()->Select ();
            return sp->GetTail ();
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

void CAdminControl::AddShape ()
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

void CAdminControl::DeleteShape ()
{
    // 図形リストが空の場合
    if (shape_num == 0)
    {
        return;
    }
    // 図形リストに含まれる Shape セルの個数が 1 の場合
    else if (shape_num == 1)
    {
        shape_head->FreeShape ();
        shape_head = NULL;
        shape_tail = NULL;
        shape_num--;
    }
    // 図形リストに含まれる Shape セルの個数が 1 より多い場合
    else
    {
        CShape* pre_sp = shape_tail->GetPre ();
        pre_sp->SetNext (NULL);
        shape_tail->FreeShape ();
        shape_tail = pre_sp;
        shape_num--;
    }
}

int CAdminControl::GetShapeNum ()
{
    return shape_num;
}

void CAdminControl::PushVertex (float new_x, float new_y)
{
    CVertex new_vertex (new_x, new_y, NULL, NULL);

    // 図形リストが空の場合
    if (shape_num == 0)
    {
        AddShape ();
    }
    // 図形リストに含まれる Shape セルの個数が 2 以上の場合
    if (shape_num >= 2)
    {
        if (IsNewVertexContained (&new_vertex) || IsNewVertexOtherCross (&new_vertex))
        {
            return;
        }
    }

    // 図形リストの最新の Shape セルに含まれる Vertex セルの個数が 3 未満（多角形を作る場合は最低 3 点が必要）の場合
    if (shape_tail->GetVertexNum () < 3)
    {
        shape_tail->AddVertex (new_x, new_y);
    }
    // 図形リストの最新の Shape セルに含まれる Vertex セルの個数が 3 以上の場合
    else
    {
        // 図形リストの最新の Shape セルに含まれる始点と追加点の距離が近い場合
        if (CMath::VertexDis (shape_tail->GetHead (), &new_vertex) < 0.1)
        {
            if (shape_tail->IsNewVertexSelfCross (&new_vertex) || IsNewShapeContaining ())
            {
                return;
            }
            else
            {
                AddShape ();
            }
        }
        // 図形リストの最新の Shape セルに含まれる始点と追加点の距離が離れている場合
        else
        {
            shape_tail->AddVertex (new_x, new_y);
        }
    }
}

void CAdminControl::PopVertex ()
{
    // 図形リストが空の場合
    if (shape_num == 0)
    {
        return;
    }
    // 図形リストが空でない場合．
    else
    {
        // 図形リストの最新の Shape セルに含まれる点リストが空の場合
        if (shape_tail->GetVertexNum () == 0)
        {
            shape_tail->DeleteVertex ();
            DeleteShape ();
        }
        // 図形リストの最新の Shape セルに含まれる点リストが空でない場合
        else
        {
            shape_tail->DeleteVertex ();
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
        for (CShape* sp = shape_head; sp != shape_tail; sp = sp->GetNext ())
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

        if (CMath::IsContained (sp, moved_vertex))
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

        if (CMath::IsContained (moved_shape, sp->GetHead ()))
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
