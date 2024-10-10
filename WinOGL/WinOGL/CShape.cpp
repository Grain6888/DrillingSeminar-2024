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


// 次のセルを指すポインタを書き込む
void CShape::SetNext (CShape* new_next)
{
    next_shape = new_next;
}
// 前のセルを指すポインタを書き込む
void CShape::SetPre (CShape* new_pre)
{
    pre_shape = new_pre;
}


// 次のセルを指すポインタを取得する
CShape* CShape::GetNext ()
{
    return next_shape;
}
// 前のセルを指すポインタを取得する
CShape* CShape::GetPre ()
{
    return pre_shape;
}


// 形状に含まれる点の数を取得する
int CShape::GetVertexNum ()
{
    return vertex_num;
}


// 形状の vertex_head を指すポインタを取得する
CVertex* CShape::GetHead ()
{
    return vertex_head;
}
// 形状の vertex_tail を指すポインタを取得する
CVertex* CShape::GetTail ()
{
    return vertex_tail;
}


// リストを解放する
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


// 左クリックでリストに点を追加
void CShape::AddVertex (float new_x, float new_y)
{
    CVertex* new_v = new CVertex;
    new_v->SetXY (new_x, new_y);
    CVertex* pre_v = vertex_tail;

    // リストが空の場合
    if (vertex_head == NULL)
    {
        vertex_head = new_v;
    }
    // 中間点または終了点（リストが空でない）の場合
    else
    {
        vertex_tail->SetNext (new_v);
        new_v->SetPre (pre_v);
    }
    vertex_tail = new_v;
    vertex_num++;

    return;
}
// 右クリックで最新の点を削除
void CShape::DeleteVertex ()
{
    CVertex* pre_vp = new CVertex;

    //開始点かつ終了点（リストが空）の場合
    if (vertex_head == NULL)
    {
        return;
    }
    //リストの要素が１つの場合
    else if (vertex_head == vertex_tail)
    {
        vertex_head->FreeVertex ();
        vertex_head = NULL;
        vertex_tail = NULL;
        vertex_num--;
    }
    //中間点または終了点（リストが空でない）の場合
    else
    {
        pre_vp = vertex_tail->GetPre ();
        pre_vp->SetNext (NULL);
        vertex_tail->FreeVertex ();
        vertex_tail = pre_vp;
        vertex_num--;
    }
}