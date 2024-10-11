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
    }

    return;
}


// 点の描画
void CAdminControl::DrawPoint (CVertex* vertex)
{
    // 点リストの最後尾を予測点としてホワイトで描画
    if (vertex == shape_tail->GetTail ())
    {
        glColor3f (1.0, 1.0, 1.0);
    }
    // それ以外の点をマゼンタで描画
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
    // 通常の点の間を実線で描画
    glColor3f (1.0, 0.0, 1.0);
    glLineWidth (2.0);
    glBegin (GL_LINE_STRIP); /*実線開始*/

    for (CVertex* vp = start; vp != end; vp = vp->GetNext ())
    {
        glVertex2f (vp->GetX (), vp->GetY ());

        // 形状リスト最後尾のセルに含まれる点リストの最後尾をつなぐ線を予測線として破線で描画
        if (vp->GetNext () == shape_tail->GetTail ())
        {
            glEnd (); /*実線終了*/

            glEnable (GL_LINE_STIPPLE); /*破線開始*/
            glLineStipple (2, 0xF0F0);
            glColor3f (1.0, 1.0, 1.0);
            glBegin (GL_LINES);
            glVertex2f (vp->GetX (), vp->GetY ());
            glVertex2f (vp->GetNext ()->GetX (), vp->GetNext ()->GetY ());
        }
    }
    glEnd ();
    glDisable (GL_LINE_STIPPLE); /*破線終了*/

    return;
}
// ループの描画
void CAdminControl::DrawLoop (CVertex* start, CVertex* end)
{
    // 形状リストの最後尾以外のセルに含まれる点リストをつなぐ線を実線で描画
    glColor3f (0.0, 1.0, 0.0);
    glLineWidth (2.0);
    glBegin (GL_LINE_LOOP);
    for (CVertex* vp = start; vp != end; vp = vp->GetNext ())
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
    if (shape_tail == NULL)
    {
        AddShape ();
    }

    // 形状リストのセルに含まれる点が3つ以下の場合
    if (shape_tail->GetVertexNum () <= 3)
    {
        shape_tail->AddVertex (new_x, new_y); /*三角形を作成するためには終点を含めて4点が少なくとも必要*/
    }
    // 形状リストのセルに追加する点が vertex_head と近い場合
    else if (CM.VertexDistance (shape_tail->GetHead (), new_x, new_y) < 0.1)
    {
        AddShape (); /*後述の AddTmpList で予測点として用いた点を形状を閉じるための点として流用するので，新規に AddVertex の必要はない*/
    }
    // 形状リストのセルに追加する点が vertex_head から離れている場合
    else
    {
        shape_tail->AddVertex (new_x, new_y); /*普通に点を追加するだけ*/
    }
}
// 形状リストに予測点を追加
void CAdminControl::AddTmpList (float new_x, float new_y)
{
    // 形状リストが空の場合はリストを新規作成
    if (shape_tail == NULL)
    {
        AddShape ();
    }

    shape_tail->DeleteVertex (); /*前回追加した AddTmpList を削除してから新しい座標の AddTmpList に更新する*/

    // 形状リストのセルに含まれる点が3つ以下の場合
    if (shape_tail->GetVertexNum () < 3)
    {
        shape_tail->AddVertex (new_x, new_y); /*三角形を作成するためには終点を含めて4点が少なくとも必要*/
    }
    // 形状リストのセルに追加する予測点が  vertex_head と近い場合
    else if (CM.VertexDistance (shape_tail->GetHead (), new_x, new_y) < 0.1)
    {
        shape_tail->AddVertex (shape_tail->GetHead ()->GetX (), shape_tail->GetHead ()->GetY ()); /*予測点が vertex_head に自動で吸着する．ただし，形状リストの新規追加は行わない*/
    }
    // 形状リストのセルに追加する予測点が  vertex_head から離れている場合
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