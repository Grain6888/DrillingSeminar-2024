#include "pch.h"
#include "CVertex.h"

CVertex::CVertex ()
{
    x = 0.0;
    y = 0.0;
    last_x = 0.0;
    last_y = 0.0;
    selected_flag = false;
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

void CVertex::SetLastXY (float x, float y)
{
    last_x = x;
    last_y = y;
}

float CVertex::GetX ()
{
    return x;
}

float CVertex::GetLastX ()
{
    return last_x;
}

float CVertex::GetY ()
{
    return y;
}

float CVertex::GetLastY ()
{
    return last_y;
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

void CVertex::SetSelection ()
{
    selected_flag = true;
}

void CVertex::SetNotSelection ()
{
    selected_flag = false;
}

bool CVertex::GetSelection ()
{
    return selected_flag;
}
