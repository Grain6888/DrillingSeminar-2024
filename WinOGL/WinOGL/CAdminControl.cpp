#include "pch.h"
#include "CAdminControl.h"

// コンストラクタ
CAdminControl::CAdminControl ()
{
    vertex_head = NULL;
}

// デストラクタ
CAdminControl::~CAdminControl ()
{
    vertex_head->FreeVertex ();
    vertex_head = NULL;
}

void CAdminControl::Draw ()
{
    if (vertex_head != NULL)
    {
        // リストの内容を描画する 
        for (CVertex* vp = vertex_head; vp != NULL; vp = vp->GetNext ())
        {
            DrawPoint (vp);

            //終了点の場合
            if (vp->GetNext () == NULL)
            {
                break;
            }
            //中間点の場合
            else
            {
                DrawLine (vp, vp->GetNext ());
            }
        }
    }

    return;
}



// 左クリックでリストに点を追加
void CAdminControl::AddVertex (float new_x, float new_y)
{
    CVertex* new_v = new CVertex;
    new_v->SetXY (new_x, new_y);

    // 開始点かつ終了点（リストが空）の場合
    if (vertex_head == NULL)
    {
        vertex_head = new_v;
    }
    // 中間点または終了点（リストが空でない）の場合
    else
    {
        for (CVertex* vp = vertex_head; vp != NULL; vp = vp->GetNext ())
        {
            if (vp->GetNext () == NULL)
            {
                vp->SetNext (new_v);
                break;
            }
        }
    }

    return;
}



// 右クリックで最新の点を削除
void CAdminControl::DeleteVertex ()
{
    CVertex* pre_vp = new CVertex;
    CVertex* latest_v = new CVertex;

    //開始点かつ終了点（リストが空）の場合
    if (vertex_head == NULL)
    {
        return;
    }
    //リストの要素が１つの場合
    else if (vertex_head->GetNext () == NULL)
    {
        vertex_head->FreeVertex ();
        vertex_head = NULL;
    }
    //中間点または終了点（リストが空でない）の場合
    else
    {
        for (CVertex* vp = vertex_head; vp != NULL; vp = vp->GetNext ())
        {
            if (vp->GetNext () == NULL)
            {
                pre_vp->SetNext (NULL);
                vp->FreeVertex ();
                return;
            }
            pre_vp = vp;
        }
    }
}

// 点の描画
void CAdminControl::DrawPoint (CVertex* vertex)
{
    // 予測点
    if (vertex->GetNext () == NULL)
    {
        glColor3f (1.0, 1.0, 1.0);
    }
    // 入力済みの点
    else
    {
        glColor3f (1.0, 0.0, 1.0);
    }
    glPointSize (10.0);
    glBegin (GL_POINTS);
    glVertex2f (vertex->GetX (), vertex->GetY ());
    glEnd ();

    return;
}

// 線の描画
void CAdminControl::DrawLine (CVertex* start, CVertex* end)
{
    // 予測線
    if (end->GetNext () == NULL)
    {
        glEnable (GL_LINE_STIPPLE);
        glLineStipple (2, 0xF0F0);
        glColor3f (1.0, 1.0, 1.0);
        glLineWidth (2.0);
        glBegin (GL_LINE_STRIP);
        glVertex2f (start->GetX (), start->GetY ());
        glVertex2f (end->GetX (), end->GetY ());
        glEnd ();
        glDisable (GL_LINE_STIPPLE);
    }
    // 入力済みの線
    else
    {
        glColor3f (1.0, 0.0, 1.0);
        glLineWidth (2.0);
        glBegin (GL_LINE_STRIP);
        glVertex2f (start->GetX (), start->GetY ());
        glVertex2f (end->GetX (), end->GetY ());
        glEnd ();
    }

    return;
}