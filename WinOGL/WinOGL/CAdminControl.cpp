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
    shape_head = NULL;
    shape_tail = NULL;
}

void CAdminControl::Draw (float new_x, float new_y)
{
    DrawExpectedPoint (new_x, new_y);

    // 図形リストが空でない場合のみ実行．
    if (shape_num != 0)
    {
        // 図形リストを順にたどる．
        for (CShape* sp = shape_head; sp != NULL; sp = sp->GetNext ())
        {
            // 図形リストに含まれる点リストを順にたどる．
            for (CVertex* vp = sp->GetHead (); vp != NULL; vp = vp->GetNext ())
            {
                DrawPoint (vp);
            }

            // 完成済みの図形（図形リストの最新の Shape セル以外）の場合
            if (sp != shape_tail)
            {
                DrawLoop (sp->GetHead (), sp->GetTail ());
            }
            // 作りかけの図形（図形リストの最新の Shape セル）の場合
            else
            {
                DrawStrip (shape_tail->GetHead (), shape_tail->GetTail ());

                // 最新の Shape セルに含まれる点リストが空でない場合のみ実行．
                if (shape_tail->GetVertexNum () > 0)
                {
                    DrawExpectedLine (shape_tail->GetTail (), new_x, new_y);
                }
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

void CAdminControl::DrawPoint (CVertex* vertex)
{
    glColor3f (1.0, 0.0, 1.0);
    glPointSize (POINTSIZE);
    glBegin (GL_POINTS);
    glVertex2f (vertex->GetX (), vertex->GetY ());
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

void CAdminControl::AddList (float new_x, float new_y)
{
    // 図形リストが空の場合
    if (shape_num == 0)
    {
        AddShape ();
    }
    // 図形リストが空でない場合
    else
    {
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
