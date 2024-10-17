#include "pch.h"
#include "CAdminControl.h"
#include "CShape.h"
#include "CMath.h"

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

void CAdminControl::Draw (float new_x, float new_y)
{
    // 図形を描画する
    if (shape_head != NULL)
    {
        // 形状リストを順にたどる
        for (CShape* sp = shape_head; sp != NULL; sp = sp->GetNext ())
        {
            // 形状リスト内のすべての点を描画する
            for (CVertex* vp = sp->GetHead (); vp != NULL; vp = vp->GetNext ())
            {
                DrawPoint (vp);
            }

            // 形状リスト内の既存（完成済み）の図形を LINE_STRIP で描画する
            if (sp != shape_tail)
            {
                DrawLoop (sp->GetHead (), sp->GetTail ());
            }
        }

        // 形状リスト内の新規（作りかけ）の図形を LINE_LOOP で描画する
        DrawLine (shape_tail->GetHead (), shape_tail->GetTail ());

        DrawExpectedLine (shape_tail->GetTail (), new_x, new_y);
    }

    DrawExpectedPoint (new_x, new_y);

    return;
}

void CAdminControl::DrawExpectedPoint (float new_x, float new_y)
{
    if (shape_tail != NULL && shape_tail->GetVertexNum () > 2 && CMath::VertexDistance (shape_tail->GetHead (), new_x, new_y) < 0.1)
    {
        glColor3f (0.0, 1.0, 1.0);
        glPointSize (15.0);
    }
    else
    {
        glColor3f (1.0, 1.0, 1.0);
        glPointSize (10.0);
    }
    glBegin (GL_POINTS);
    glVertex2f (new_x, new_y);
    glEnd ();
}


void CAdminControl::DrawExpectedLine (CVertex* start, float end_x, float end_y)
{
    if (start != NULL)
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
}


// 点の描画
void CAdminControl::DrawPoint (CVertex* vertex)
{
    glColor3f (1.0, 0.0, 1.0);
    glPointSize (10.0);
    glBegin (GL_POINTS);
    glVertex2f (vertex->GetX (), vertex->GetY ());
    glEnd ();

    return;
}
// 線の描画
void CAdminControl::DrawLine (CVertex* start, CVertex* end)
{
    glColor3f (1.0, 0.0, 1.0);
    glLineWidth (2.0);
    glBegin (GL_LINE_STRIP);

    for (CVertex* vp = start; vp != NULL; vp = vp->GetNext ())
    {
        glVertex2f (vp->GetX (), vp->GetY ());
    }
    glEnd ();

    return;
}
// ループの描画
void CAdminControl::DrawLoop (CVertex* start, CVertex* end)
{
    // 形状リストの最後尾以外のセルに含まれる点リストをつなぐ線を実線で描画
    glColor3f (0.0, 1.0, 0.0);
    glLineWidth (2.0);
    glBegin (GL_LINE_LOOP);
    for (CVertex* vp = start; vp != NULL; vp = vp->GetNext ())
    {
        glVertex2f (vp->GetX (), vp->GetY ());
    }
    glEnd ();
}


// 形状リストにセルを追加
void CAdminControl::AddShape ()
{
    CShape* new_s = new CShape;
    CShape* pre_s = shape_tail;


    // 形状リストが空の場合
    if (shape_head == NULL)
    {
        shape_head = new_s;
    }
    // 形状リストが空でない場合
    else
    {
        shape_tail->SetNext (new_s); /*最後尾の更新*/
        new_s->SetPre (pre_s);
    }

    shape_tail = new_s; /*最後尾は必ず追加された点*/

    return;
}
// 形状リストの最新のセルを削除
void CAdminControl::DeleteShape ()
{
    // 形状リストが空の場合
    if (shape_head == NULL)
    {
        return;
    }
    // 形状リストのセルが1つの場合
    else if (shape_head == shape_tail)
    {
        shape_head->FreeShape (); /*セルを開放するだけ*/
        shape_head = NULL;
        shape_tail = NULL;
    }
    // 形状リストのセルが2つ以上の場合
    else
    {
        CShape* pre_sp = shape_tail->GetPre (); /*最後尾の更新*/
        pre_sp->SetNext (NULL);
        shape_tail->FreeShape ();
        shape_tail = pre_sp;

    }
}


// 形状リストに点を追加
void CAdminControl::AddList (float new_x, float new_y)
{
    // 形状リストが空の場合はリストを新規作成
    if (shape_head == NULL)
    {
        AddShape ();
    }
    else if (shape_head != shape_tail)
    {
        for (CShape* sp = shape_head; sp != shape_tail && shape_tail->GetVertexNum () > 0; sp = sp->GetNext ())
        {
            if (CMath::OtherCross (sp, shape_tail, new_x, new_y))
            {
                return;
            }
        }
    }


    // 形状リストのセルに含まれる点が3つ未満の場合
    if (shape_tail->GetVertexNum () < 3)
    {
        shape_tail->AddVertex (new_x, new_y); /*三角形を作成するためには3点が少なくとも必要*/
    }
    // 形状リストのセルに追加する点が vertex_head と近い場合
    else if (CMath::VertexDistance (shape_tail->GetHead (), new_x, new_y) < 0.1)
    {
        for (CVertex* vp = shape_tail->GetHead ()->GetNext (); vp != shape_tail->GetTail ()->GetPre (); vp = vp->GetNext ())
        {
            if (CMath::SelfCross (shape_tail->GetTail (), shape_tail->GetHead (), vp, vp->GetNext ()))
            {
                return;
            }
        }
        AddShape (); /*後述の AddTmpList で予測点として用いた点を形状を閉じるための点として流用するので，新規に AddVertex の必要はない*/
    }
    // 形状リストのセルに追加する点が vertex_head から離れている場合
    else
    {
        shape_tail->AddVertex (new_x, new_y); /*普通に点を追加するだけ*/
    }
}

// 形状リストから点を削除
void CAdminControl::SubList ()
{
    // 形状リストが空の場合
    if (shape_tail == NULL)
    {
        return;
    }
    // 形状リスト内のセルに含まれる点が1つの場合
    else if (shape_tail->GetHead () == shape_tail->GetTail ())
    {
        shape_tail->DeleteVertex (); /*セルごと削除*/
        DeleteShape ();
    }
    // 形状リスト内のセルに含まれる点が2つ以上の場合
    else
    {
        shape_tail->DeleteVertex (); /*セル内の点のみ削除*/
    }
}
