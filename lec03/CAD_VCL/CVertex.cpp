#include "CVertex.h"

CVertex::CVertex ()
{
    x = 0;
    y = 0;
    next_vertex = NULL;
}

CVertex::CVertex (int new_x, int new_y, CVertex* new_next)
{
    SetXY (new_x, new_y);
    SetNext (new_next);
}

CVertex::~CVertex ()
{ }

// 頂点のXY座標を書き込む
void CVertex::SetXY (int new_x, int new_y)
{
    if (new_x < 0)
    {
        new_x = new_x * (-1);
    }
    x = new_x;
    if (new_y < 0)
    {
        new_y = new_y * (-1);
    }
    y = new_y;
}

// 頂点のX座標を読み込む
int CVertex::GetX ()
{
    return x;
}

// 頂点のY座標を読み込む
int CVertex::GetY ()
{
    return y;
}

// 次の頂点リストを指すポインタを書き込む
void CVertex::SetNext (CVertex* new_next)
{
    next_vertex = new_next;
}

// 次の頂点リストを指すポインタを読み込む
CVertex* CVertex::GetNext ()
{
    return next_vertex;
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
