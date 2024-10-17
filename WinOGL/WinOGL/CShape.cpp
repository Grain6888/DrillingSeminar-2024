#include "pch.h"
#include "CShape.h"
#include "CVertex.h"

CShape::CShape ()
{
    vertex_head = NULL;
    vertex_tail = NULL;
    next_shape = NULL;
    pre_shape = NULL;
    vertex_num = 0;
};
CShape::CShape (CShape* new_next, CShape* new_pre)
{
    SetNext (new_next);
    SetPre (new_pre);
};


CShape::~CShape ()
{
    vertex_head->FreeVertex ();
    vertex_head = NULL;
    vertex_tail = NULL;
};


// 形状リストの次のセルを指すポインタを書き込む
void CShape::SetNext (CShape* new_next)
{
    next_shape = new_next;
}
// 形状リストの前のセルを指すポインタを書き込む
void CShape::SetPre (CShape* new_pre)
{
    pre_shape = new_pre;
}


// 形状リストの次のセルを指すポインタを取得する
CShape* CShape::GetNext ()
{
    return next_shape;
}
// 形状リストの前のセルを指すポインタを取得する
CShape* CShape::GetPre ()
{
    return pre_shape;
}


// 形状リストのセルに含まれる点の数を取得する
int CShape::GetVertexNum ()
{
    return vertex_num;
}


// 形状リストのセルに含まれる点リストの先頭を指すポインタを取得する
CVertex* CShape::GetHead ()
{
    return vertex_head;
}
// 形状リストのセルに含まれる点リストの最後を指すポインタを取得する
CVertex* CShape::GetTail ()
{
    return vertex_tail;
}


// 形状リストを解放する
void CShape::FreeShape ()
{
    CShape* nowS = this;
    while (nowS != NULL)
    {
        CShape* del_cell = nowS;
        nowS = nowS->GetNext ();
        delete del_cell;
    }
}


// 点リストにセルを追加
void CShape::AddVertex (float new_x, float new_y)
{
    CVertex* new_v = new CVertex;
    new_v->SetXY (new_x, new_y);
    CVertex* pre_v = vertex_tail;

    // 点リストが空の場合
    if (vertex_head == NULL)
    {
        vertex_head = new_v;
    }
    // 点リストのセルが3つ以下（辺が2つまで）の場合
    else if (vertex_num < 3)
    {
        vertex_tail->SetNext (new_v); /*最後尾の更新*/
        new_v->SetPre (pre_v);
    }
    // 点リストのセルが4つ以上（辺が3つから）の場合
    else
    {
        for (CVertex* vp = vertex_head; vp != vertex_tail->GetPre (); vp = vp->GetNext ())
        {
            if (CM.SelfCross (vertex_tail, new_v, vp, vp->GetNext ()))
            {
                return;
            }
        }

        vertex_tail->SetNext (new_v); /*最後尾の更新*/
        new_v->SetPre (pre_v);
    }
    vertex_tail = new_v; /*最後尾は必ず追加された点*/
    vertex_num++;        /*形状に含まれる点の数を1つ増やす*/

    return;
}
// 点リストの最新の点を削除
void CShape::DeleteVertex ()
{
    // 点リストが空の場合
    if (vertex_head == NULL)
    {
        return;
    }
    // 点リストのセルが1つの場合
    else if (vertex_head == vertex_tail)
    {
        vertex_head->FreeVertex ();
        vertex_head = NULL;
        vertex_tail = NULL;
        vertex_num--;
    }
    // 点リストのセルが2つ以上の場合
    else
    {
        CVertex* pre_vp = vertex_tail->GetPre ();
        pre_vp->SetNext (NULL);
        vertex_tail->FreeVertex ();
        vertex_tail = pre_vp;
        vertex_num--;
    }
}