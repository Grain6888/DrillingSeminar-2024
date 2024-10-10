#include "pch.h"
#include "CAdminControl.h"
#include "CShape.h"

// コンストラクタ
CAdminControl::CAdminControl ()
{
    shape_head = NULL;
    shape_tail = NULL;
}

// デストラクタ
CAdminControl::~CAdminControl ()
{
    shape_head->FreeShape ();
    shape_head = NULL;
    shape_tail = NULL;
}

void CAdminControl::Draw ()
{
    if (shape_head != NULL)
    {
        for (CShape* sp = shape_head; sp != NULL; sp = sp->GetNext ())
        {
            // リストの内容を描画する
            for (CVertex* vp = sp->GetHead (); vp != NULL; vp = vp->GetNext ())
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
    }

    return;
}


// 点の描画
void CAdminControl::DrawPoint (CVertex* vertex)
{
    // 予測点
    if (vertex == shape_tail->GetTail ())
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
    if (end == shape_tail->GetTail ())
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


// 左クリックでリストに形状を追加
void CAdminControl::AddShape ()
{
    CShape* new_s = new CShape;
    CShape* pre_s = shape_tail;

    // 形状リストが空の場合
    if (shape_head == NULL)
    {
        shape_head = new_s;
    }
    // 中間点または終了点（リストが空でない）の場合
    else
    {
        shape_tail->SetNext (new_s);
        new_s->SetPre (pre_s);
    }
    shape_tail = new_s;

    return;
}
// 右クリックで最新の形状を削除
void CAdminControl::DeleteShape ()
{
    // 形状リストが空の場合
    if (shape_head == NULL)
    {
        return;
    }
    // 形状リストの要素が１つの場合
    else if (shape_head == shape_tail)
    {
        shape_head->FreeShape ();
        shape_head = NULL;
        shape_tail = NULL;
    }
    // 中間点または終了点（リストが空でない）の場合
    else
    {
        CShape* pre_sp = shape_tail->GetPre ();
        pre_sp->SetNext (NULL);
        shape_tail->FreeShape ();
        shape_tail = pre_sp;
    }
}


// 形状リストに点を追加する
void CAdminControl::AddList (float new_x, float new_y)
{
    // 形状リストが空の場合のみ実行
    if (shape_tail == NULL)
    {
        AddShape ();
    }

    // 点が3以下の場合
    if (shape_tail->GetVertexNum () < 3)
    {
        shape_tail->AddVertex (new_x, new_y);
    }
    // 最後の点が vertex_head と近い場合
    else if (CM.VertexDistance (shape_tail->GetHead (), new_x, new_y) < 0.1)
    {
        shape_tail->AddVertex (shape_tail->GetHead ()->GetX (), shape_tail->GetHead ()->GetY ());

        AddShape ();
    }
    // 最後の点が vertex_head から離れている場合
    else
    {
        shape_tail->AddVertex (new_x, new_y);
    }
}


// 形状リストに予測点を追加する
void CAdminControl::AddTmpList (float new_x, float new_y)
{
    // 形状リストが空の場合のみ実行
    if (shape_tail == NULL)
    {
        AddShape ();
    }

    shape_tail->DeleteVertex ();
    // 点が3以下の場合
    if (shape_tail->GetVertexNum () < 3)
    {
        shape_tail->AddVertex (new_x, new_y);
    }
    // 最後の点が vertex_head と近い場合
    else if (CM.VertexDistance (shape_tail->GetHead (), new_x, new_y) < 0.1)
    {
        shape_tail->AddVertex (shape_tail->GetHead ()->GetX (), shape_tail->GetHead ()->GetY ());
    }
    // 最後の点が vertex_head から離れている場合
    else
    {
        shape_tail->AddVertex (new_x, new_y);
    }
}


// 形状リストから点を削除する
void CAdminControl::SubList ()
{
    // 形状リストが空の場合
    if (shape_tail == NULL)
    {
        return;
    }
    // 形状リストが空でない場合
    else
    {
        if (shape_tail->GetHead () == shape_tail->GetTail ())
        {
            shape_tail->DeleteVertex ();
            DeleteShape ();
        }
        else
        {
            shape_tail->DeleteVertex ();
        }
    }
}