#include "pch.h"
#include "CShape.h"
#include "CVertex.h"
#include "CMath.h"

CShape::CShape ()
{
    vertex_head = NULL;
    vertex_tail = NULL;
    next_shape = NULL;
    pre_shape = NULL;
    vertex_num = 0;
    chose_vertex = NULL;
    last_vertex_x = 0.0;
    last_vertex_y = 0.0;
    chose_strip = NULL;
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
    chose_vertex = NULL;
    chose_strip = NULL;
};

void CShape::SetNext (CShape* new_next)
{
    next_shape = new_next;
}

void CShape::SetPre (CShape* new_pre)
{
    pre_shape = new_pre;
}

CShape* CShape::GetNext ()
{
    return next_shape;
}

CShape* CShape::GetPre ()
{
    return pre_shape;
}

int CShape::GetVertexNum ()
{
    return vertex_num;
}

void CShape::SetChoseVertex (CVertex* vp)
{
    chose_vertex = vp;
}

void CShape::SetLastVertexXY (float last_x, float last_y)
{
    last_vertex_x = last_x;
    last_vertex_y = last_y;
}

CVertex* CShape::GetChoseVertex ()
{
    return chose_vertex;
}

float CShape::GetLastVertexX ()
{
    return last_vertex_x;
}

float CShape::GetLastVertexY ()
{
    return last_vertex_y;
}

void CShape::SetChoseStrip (CVertex* sp_s)
{
    chose_strip = sp_s;
}

CVertex* CShape::GetChoseStrip ()
{
    return chose_strip;
}

CVertex* CShape::GetHead ()
{
    return vertex_head;
}

CVertex* CShape::GetTail ()
{
    return vertex_tail;
}

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

void CShape::AddVertex (float new_x, float new_y)
{
    CVertex* new_v = new CVertex;
    new_v->SetXY (new_x, new_y);
    CVertex* pre_v = vertex_tail;

    // 点リストが空の場合
    if (vertex_num == 0)
    {
        vertex_head = new_v;
    }
    // 点リストに含まれる Vertex セルの個数が 2 以下の場合
    else if (vertex_num <= 2)
    {
        vertex_tail->SetNext (new_v);
        new_v->SetPre (pre_v);
    }
    // 点リストに含まれる Vertex セルの個数が 2 より多い場合
    else
    {
        // 自身の Vertex セル（vertex_tail）を含む辺（始点: vertex_tail から一つ古いセル → 終点: vertex_tail）以外を対象に，自交差判定を行う．
        for (CVertex* vp = vertex_head; vp != vertex_tail->GetPre (); vp = vp->GetNext ())
        {
            // 自交差している場合は点の追加をキャンセル．
            if (CMath::IsSelfCrossing (vertex_tail, new_v, vp, vp->GetNext ()))
            {
                new_v->FreeVertex ();
                return;
            }
        }
        vertex_tail->SetNext (new_v);
        new_v->SetPre (pre_v);
    }
    vertex_tail = new_v;
    vertex_num++;
}

void CShape::DeleteVertex ()
{
    // 点リストが空の場合
    if (vertex_num == 0)
    {
        return;
    }
    // 点リストに含まれる Vertex セルの個数が 1 の場合
    else if (vertex_num == 1)
    {
        vertex_head->FreeVertex ();
        vertex_head = NULL;
        vertex_tail = NULL;
        vertex_num--;
    }
    // 点リストに含まれる Vertex セルの個数が 1 より多い場合
    else
    {
        CVertex* pre_vp = vertex_tail->GetPre ();
        pre_vp->SetNext (NULL);
        vertex_tail->FreeVertex ();
        vertex_tail = pre_vp;
        vertex_num--;
    }
}