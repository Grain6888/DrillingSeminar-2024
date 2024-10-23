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

void CVertex::SetXY (float new_x, float new_y)
{
    x = new_x;
    y = new_y;
}

float CVertex::GetX ()
{
    return x;
}

float CVertex::GetY ()
{
    return y;
}

void CVertex::SetNext (CVertex* new_next)
{
    next_vertex = new_next;
}

void CVertex::SetPre (CVertex* new_pre)
{
    pre_vertex = new_pre;
}

CVertex* CVertex::GetNext ()
{
    return next_vertex;
}

CVertex* CVertex::GetPre ()
{
    return pre_vertex;
}

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