#include "pch.h"
#include "CAdminControl.h"
#include "CShape.h"
#include "CMath.h"

CAdminControl::CAdminControl ()
{
    shape_head = NULL;
    shape_tail = NULL;
    shape_num = 0;
    chose_shape = NULL;
}

CAdminControl::~CAdminControl ()
{
    shape_head->FreeShape ();
    shape_head = NULL;
    shape_tail = NULL;
    chose_shape = NULL;
}

void CAdminControl::Draw (float new_x, float new_y)
{
    DrawExpectedPoint (new_x, new_y);

    if (AxisFlag)
    {
        DrawAxis ();
    }

    // 図形リストが空でない場合のみ実行．
    if (shape_num != 0)
    {
        // 図形リストを順にたどる．
        for (CShape* sp = shape_head; sp != NULL; sp = sp->GetNext ())
        {
            // 図形リストに含まれる点リストを順にたどる．
            for (CVertex* vp = sp->GetHead (); vp != NULL; vp = vp->GetNext ())
            {
                if (vp != sp->GetChoseVertex ())
                {
                    DrawPoint (vp);
                }
                else
                {
                    DrawChoseVertex (vp);
                }
            }

            // 完成済みの図形（図形リストの最新の Shape セル以外）の場合
            if (sp != shape_tail)
            {
                if (sp != chose_shape)
                {
                    DrawLoop (sp->GetHead (), sp->GetTail ());
                }
                else
                {
                    DrawChoseLoop (sp->GetHead (), sp->GetTail ());
                }
            }
            // 作りかけの図形（図形リストの最新の Shape セル）の場合
            else
            {
                DrawStrip (shape_tail->GetHead (), shape_tail->GetTail ());

                // 最新の Shape セルに含まれる点リストが空でない場合のみ実行．
                if (shape_tail->GetVertexNum () > 0 && EditModeFlag)
                {
                    DrawExpectedLine (shape_tail->GetTail (), new_x, new_y);
                }
            }

            for (CVertex* vp = sp->GetHead (); vp != sp->GetTail (); vp = vp->GetNext ())
            {
                if (vp == sp->GetChoseStrip ())
                {
                    DrawChoseStrip (vp, vp->GetNext ());
                }
            }
            if (sp->GetTail () != NULL && sp->GetTail () == sp->GetChoseStrip ())
            {
                DrawChoseStrip (sp->GetTail (), sp->GetHead ());
            }
        }
    }
}

void CAdminControl::DrawExpectedPoint (float new_x, float new_y)
{
    // 図形リストが空でなく，最新の Shape セルに含まれる Vertex セルの個数が 3 以上で，マウスポインタの座標が始点の座標と近い場合
    if (shape_num != 0 && shape_tail->GetVertexNum () >= 3 && CMath::VertexDistance (shape_tail->GetHead (), new_x, new_y) < 0.1)
    {
        glColor3f (0.0, 1.0, 1.0);
        glPointSize (POINTSIZE);
    }
    // 通常の場合
    else
    {
        glColor3f (1.0, 1.0, 1.0);
        glPointSize (POINTSIZE);
    }
    glBegin (GL_POINTS);
    glVertex2f (new_x, new_y);
    glEnd ();
}

void CAdminControl::DrawExpectedLine (CVertex* start, float end_x, float end_y)
{
    glEnable (GL_LINE_STIPPLE);
    glLineStipple (2, 0xF0F0);
    glColor3f (1.0, 1.0, 1.0);
    glBegin (GL_LINES);
    glVertex2f (start->GetX (), start->GetY ());
    glVertex2f (end_x, end_y);
    glEnd ();
    glDisable (GL_LINE_STIPPLE);
}

void CAdminControl::SearchNearestVertex (float mouse_x, float mouse_y)
{
    if (shape_num != 0)
    {
        for (CShape* sp = shape_head; sp != NULL; sp = sp->GetNext ())
        {
            for (CVertex* vp = sp->GetHead (); vp != NULL; vp = vp->GetNext ())
            {
                if (CMath::VertexDistance (vp, mouse_x, mouse_y) < 0.1)
                {
                    sp->SetChoseVertex (vp);
                    return;
                }
                else
                {
                    sp->SetChoseVertex (NULL);
                }
            }
        }
    }
}

void CAdminControl::SearchNearestShape (float mouse_x, float mouse_y)
{
    if (shape_num > 1)
    {
        for (CShape* sp = shape_head; sp != shape_tail; sp = sp->GetNext ())
        {
            if (CMath::InclusionDetect (shape_head, sp, mouse_x, mouse_y))
            {
                chose_shape = sp;
                return;
            }
            else
            {
                chose_shape = NULL;
            }
        }
    }
}

void CAdminControl::SearchNearestStrip (float mouse_x, float mouse_y)
{
    if (shape_num != 0)
    {
        for (CShape* sp = shape_head; sp != NULL && sp->GetVertexNum () != 0; sp = sp->GetNext ())
        {
            for (CVertex* vp = sp->GetHead (); vp != sp->GetTail (); vp = vp->GetNext ())
            {
                if (CMath::StripDistance (mouse_x, mouse_y, vp, vp->GetNext ()) < 0.1 && CMath::VertexDistance (vp, mouse_x, mouse_y) >= 0.1 && CMath::VertexDistance (vp->GetNext (), mouse_x, mouse_y) >= 0.1)
                {
                    sp->SetChoseStrip (vp);
                    return;
                }
                else
                {
                    sp->SetChoseStrip (NULL);
                }
            }
            if (CMath::StripDistance (mouse_x, mouse_y, sp->GetTail (), sp->GetHead ()) < 0.1 && CMath::VertexDistance (sp->GetTail (), mouse_x, mouse_y) >= 0.1 && CMath::VertexDistance (sp->GetHead (), mouse_x, mouse_y) >= 0.1)
            {
                sp->SetChoseStrip (sp->GetTail ());
                return;
            }
            else
            {
                sp->SetChoseStrip (NULL);
            }
        }
    }
}

void CAdminControl::DrawChoseVertex (CVertex* vp)
{
    glColor3f (0.0, 1.0, 0.0);
    glPointSize (POINTSIZE);
    glBegin (GL_POINTS);
    glVertex2f (vp->GetX (), vp->GetY ());
    glEnd ();
}

void CAdminControl::DrawPoint (CVertex* vertex)
{
    glColor3f (1.0, 0.0, 1.0);
    glPointSize (POINTSIZE);
    glBegin (GL_POINTS);
    glVertex2f (vertex->GetX (), vertex->GetY ());
    glEnd ();
}

void CAdminControl::DrawChoseStrip (CVertex* sp_s, CVertex* sp_e)
{
    glColor3f (0.0, 1.0, 0.0);
    glLineWidth (LINEWIDTH);
    glBegin (GL_LINES);
    glVertex2f (sp_s->GetX (), sp_s->GetY ());
    glVertex2f (sp_e->GetX (), sp_e->GetY ());
    glEnd ();
}

void CAdminControl::DrawStrip (CVertex* start, CVertex* end)
{
    glColor3f (1.0, 0.0, 1.0);
    glLineWidth (LINEWIDTH);
    glBegin (GL_LINE_STRIP);
    for (CVertex* vp = start; vp != NULL; vp = vp->GetNext ())
    {
        glVertex2f (vp->GetX (), vp->GetY ());
    }
    glEnd ();
}

void CAdminControl::DrawLoop (CVertex* start, CVertex* end)
{
    glColor3f (1.0, 1.0, 0.0);
    glLineWidth (LINEWIDTH);
    glBegin (GL_LINE_LOOP);
    for (CVertex* vp = start; vp != NULL; vp = vp->GetNext ())
    {
        glVertex2f (vp->GetX (), vp->GetY ());
    }
    glEnd ();
}

void CAdminControl::DrawChoseLoop (CVertex* start, CVertex* end)
{
    glColor3f (0.0, 1.0, 0.0);
    glLineWidth (LINEWIDTH);
    glBegin (GL_LINE_LOOP);
    for (CVertex* vp = start; vp != NULL; vp = vp->GetNext ())
    {
        glVertex2f (vp->GetX (), vp->GetY ());
    }
    glEnd ();
}

void CAdminControl::AddShape ()
{
    CShape* new_s = new CShape;
    CShape* pre_s = shape_tail;


    // 図形リストが空の場合
    if (shape_num == 0)
    {
        shape_head = new_s;
    }
    // 図形リストが空でない場合
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

void CAdminControl::SetChoseShape (CShape* sp)
{
    chose_shape = sp;
}

CShape* CAdminControl::GetChoseShape ()
{
    return chose_shape;
}

void CAdminControl::AddList (float new_x, float new_y)
{
    // 図形リストが空の場合
    if (shape_num == 0)
    {
        AddShape ();
    }
    // 図形リストに含まれる Shape セルの個数が 1 より多い場合
    else if (shape_num > 1)
    {
        //内包判定
        if (CMath::InclusionDetect (shape_head, shape_tail->GetPre (), new_x, new_y))
        {
            return;
        }

        // 図形リストの最新の Shape セルに含まれる点リストが空でない場合
        if (shape_tail->GetVertexNum () > 0)
        {
            // 自身の Shape セル（shape_tail）以外を対象に，他交差判定を行う．
            for (CShape* sp = shape_head; sp != shape_tail; sp = sp->GetNext ())
            {
                if (CMath::OtherCrossDetect (sp, shape_tail, new_x, new_y))
                {
                    return;
                }
            }
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
        if (CMath::VertexDistance (shape_tail->GetHead (), new_x, new_y) < 0.1)
        {
            // いわゆる砂時計の形を防ぐために，自交差判定を行う．
            for (CVertex* vp = shape_tail->GetHead ()->GetNext (); vp != shape_tail->GetTail ()->GetPre (); vp = vp->GetNext ())
            {
                if (CMath::CrossDetect (shape_tail->GetTail (), shape_tail->GetHead (), vp, vp->GetNext ()))
                {
                    return;
                }
            }

            for (CShape* sp = shape_head; sp != shape_tail; sp = sp->GetNext ())
            {
                if (CMath::InclusionDetect (shape_tail, shape_tail, sp->GetHead ()->GetX (), sp->GetHead ()->GetY ()))
                {
                    return;
                }
            }
            AddShape ();
        }
        // 図形リストの最新の Shape セルに含まれる始点と追加点の距離が離れている場合
        else
        {
            shape_tail->AddVertex (new_x, new_y);
        }
    }
}

void CAdminControl::SubList ()
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
        POINTSIZE += 0.5;
        LINEWIDTH += 0.5;
    }
}

void CAdminControl::DrawSizeDown ()
{
    if (LINEWIDTH >= 1.0)
    {
        POINTSIZE -= 0.5;
        LINEWIDTH -= 0.5;
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

bool CAdminControl::GetAxis ()
{
    return AxisFlag;
}

void CAdminControl::SwitchEditMode ()
{
    EditModeFlag = !EditModeFlag;
}

bool CAdminControl::GetEditMode ()
{
    return EditModeFlag;
}
