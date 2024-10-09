#include "pch.h"
#include "CVertex.h"

CVertex::CVertex ()
{
    x = 0.0;
    y = 0.0;
    next_vertex = NULL;
    pre_vertex = NULL;
};


CVertex::CVertex (float new_x, float new_y, CVertex* new_next, CVertex* new_pre)
{
    SetXY (new_x, new_y);
    SetNext (new_next);
    SetPre (new_pre);
};

CVertex::~CVertex ()
{ };


// 頂点のXY座標を書き込む
void CVertex::SetXY (float new_x, float new_y)
{
    x = new_x;
    y = new_y;
}


// x座標をセルから取得する
float CVertex::GetX ()
{
    return x;
}
// y座標をセルから取得する
float CVertex::GetY ()
{
    return y;
}


// 次のセルを指すポインタを書き込む
void CVertex::SetNext (CVertex* new_next)
{
    next_vertex = new_next;
}
// 前のセルを指すポインタを書き込む
void CVertex::SetPre (CVertex* new_pre)
{
    pre_vertex = new_pre;
}


// 次のセルを指すポインタを取得する
CVertex* CVertex::GetNext ()
{
    return next_vertex;
}
// 前のセルを指すポインタを取得する
CVertex* CVertex::GetPre ()
{
    return pre_vertex;
}


// リストを解放する
void CVertex::FreeVertex ()
{
    CVertex* nowV = this;
    while (nowV != NULL)
    {
        CVertex* del_cell = nowV;
        nowV = nowV->GetNext ();
        delete del_cell;
    }
}