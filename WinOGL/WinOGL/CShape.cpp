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
};

CShape::CShape (CShape* new_next, CShape* new_pre)
{
    SetNext (new_next);
    SetPre (new_pre);
};

CShape::~CShape ()
{
    vertex_head->FreeVertex ();
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

void CShape::PushVertex (float new_x, float new_y)
{
    CVertex* new_vertex = new CVertex;
    new_vertex->SetXY (new_x, new_y);
    CVertex* pre_vertex = vertex_tail;

    // 点リストが空の場合
    if (vertex_num == 0)
    {
        vertex_head = new_vertex;
    }
    // 点リストに含まれる Vertex セルの個数が 2 以下の場合
    else if (vertex_num <= 2)
    {
        vertex_tail->SetNext (new_vertex);
        new_vertex->SetPre (pre_vertex);
    }
    // 点リストに含まれる Vertex セルの個数が 2 より多い場合
    else
    {
        if (IsNewVertexSelfCross (new_vertex))
        {
            new_vertex->FreeVertex ();
            return;
        }
        vertex_tail->SetNext (new_vertex);
        new_vertex->SetPre (pre_vertex);
    }
    vertex_tail = new_vertex;
    vertex_num++;
}

void CShape::InsertVertex (CVertex* pre_vertex, float new_x, float new_y, CVertex* next_vertex)
{
    CVertex* new_vertex = new CVertex;
    new_vertex->SetXY (new_x, new_y);

    pre_vertex->SetNext (new_vertex);

    new_vertex->SetPre (pre_vertex);
    new_vertex->SetNext (next_vertex);

    next_vertex->SetPre (new_vertex);

    vertex_num++;
}

void CShape::PopVertex ()
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

void CShape::RemoveVertex (CVertex* remove_vertex)
{
    if (remove_vertex == NULL)
    {
        return;
    }
    else if (remove_vertex == vertex_head)
    {
        CVertex* next_vertex = remove_vertex->GetNext ();
        next_vertex->SetPre (NULL);
        remove_vertex->SetNext (NULL);
        vertex_head = next_vertex;
        remove_vertex->FreeVertex ();
        vertex_num--;
    }
    else if (remove_vertex == vertex_tail)
    {
        PopVertex ();
    }
    else
    {
        CVertex* pre_vertex = remove_vertex->GetPre ();
        CVertex* next_vertex = remove_vertex->GetNext ();
        pre_vertex->SetNext (next_vertex);
        next_vertex->SetPre (pre_vertex);
        remove_vertex->SetPre (NULL);
        remove_vertex->SetNext (NULL);
        remove_vertex->FreeVertex ();
        vertex_num--;
    }
}

bool CShape::IsNewVertexSelfCross (CVertex* new_vertex)
{
    // 自交差をチェック
    if (vertex_num >= 3)
    {
        for (CVertex* vp = vertex_head; vp != vertex_tail->GetPre (); vp = vp->GetNext ())
        {
            if (CMath::IsLineCrossing (vp, vp->GetNext (), vertex_tail, new_vertex) && CMath::VertexDis (new_vertex, vertex_head) != 0.0)
            {
                return true;
            }
        }
    }

    // 砂時計をチェック
    if (vertex_num >= 4)
    {
        for (CVertex* vp = vertex_head->GetNext (); vp != vertex_tail->GetPre (); vp = vp->GetNext ())
        {
            if (CMath::VertexDis (vertex_head, new_vertex) < 0.1 && CMath::IsLineCrossing (vp, vp->GetNext (), vertex_head, vertex_tail))
            {
                return true;
            }
        }
    }

    return false;
}

bool CShape::IsMovedVertexSelfCross (CVertex* moved_vertex)
{
    if (vertex_num > 3)
    {
        if (moved_vertex == vertex_head)
        {
            for (CVertex* vp = vertex_tail->GetPre (); vp != vertex_head->GetNext (); vp = vp->GetPre ())
            {
                if (ClosedFlag && CMath::IsLineCrossing (vertex_head, vertex_tail, vp, vp->GetPre ()))
                {
                    return true;
                }
            }
            for (CVertex* vp = vertex_tail; vp != vertex_head->GetNext ()->GetNext (); vp = vp->GetPre ())
            {
                if (CMath::IsLineCrossing (vertex_head, vertex_head->GetNext (), vp, vp->GetPre ()))
                {
                    return true;
                }
            }
        }
        else if (moved_vertex == vertex_tail)
        {
            for (CVertex* vp = vertex_head->GetNext (); vp != vertex_tail->GetPre (); vp = vp->GetNext ())
            {
                if (ClosedFlag && CMath::IsLineCrossing (vertex_head, vertex_tail, vp, vp->GetNext ()))
                {
                    return true;
                }
            }
            for (CVertex* vp = vertex_head; vp != vertex_tail->GetPre ()->GetPre (); vp = vp->GetNext ())
            {
                if (CMath::IsLineCrossing (vertex_tail, vertex_tail->GetPre (), vp, vp->GetNext ()))
                {
                    return true;
                }
            }
        }
        else
        {
            for (CVertex* vp = vertex_head; vp != vertex_tail; vp = vp->GetNext ())
            {
                if (vp == moved_vertex->GetPre () || vp == moved_vertex || vp == moved_vertex->GetNext ())
                {
                    continue;
                }
                if (CMath::IsLineCrossing (moved_vertex, moved_vertex->GetNext (), vp, vp->GetNext ()))
                {
                    return true;
                }
            }
            if (moved_vertex->GetNext () != vertex_tail)
            {
                if (ClosedFlag && CMath::IsLineCrossing (moved_vertex, moved_vertex->GetNext (), vertex_tail, vertex_head))
                {
                    return true;
                }
            }
            for (CVertex* vp = vertex_tail; vp != vertex_head; vp = vp->GetPre ())
            {
                if (vp == moved_vertex->GetNext () || vp == moved_vertex || vp == moved_vertex->GetPre ())
                {
                    continue;
                }
                if (CMath::IsLineCrossing (moved_vertex, moved_vertex->GetPre (), vp, vp->GetPre ()))
                {
                    return true;
                }
            }
            if (moved_vertex->GetPre () != vertex_head)
            {
                if (ClosedFlag && CMath::IsLineCrossing (moved_vertex, moved_vertex->GetPre (), vertex_head, vertex_tail))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool CShape::IsRemoveVertexSelfCross (CVertex* remove_vertex)
{
    CVertex* pre_vertex = new CVertex;
    CVertex* next_vertex = new CVertex;

    if (remove_vertex == vertex_head)
    {
        pre_vertex = vertex_tail;
        next_vertex = remove_vertex->GetNext ();
    }
    else if (remove_vertex == vertex_tail)
    {
        pre_vertex = remove_vertex->GetPre ();
        next_vertex = vertex_head;
    }
    else
    {
        pre_vertex = remove_vertex->GetPre ();
        next_vertex = remove_vertex->GetNext ();
    }

    for (CVertex* vp = vertex_head; vp != vertex_tail && vp != NULL; vp = vp->GetNext ())
    {
        if (vp == pre_vertex)
        {
            vp = vp->GetNext ();
        }
        else if (vp == pre_vertex->GetPre ())
        {
            continue;
        }
        else if (vp == next_vertex)
        {
            continue;
        }
        else if (vp == remove_vertex)
        {
            continue;
        }
        else if (CMath::IsLineCrossing (vp, vp->GetNext (), pre_vertex, next_vertex))
        {
            return true;
        }
    }
    if (pre_vertex != vertex_head && pre_vertex != vertex_tail && next_vertex != vertex_head && next_vertex != vertex_tail && CMath::IsLineCrossing (vertex_head, vertex_tail, pre_vertex, next_vertex))
    {
        return true;
    }

    return false;
}

void CShape::SelectAllVertex ()
{
    for (CVertex* vp = vertex_head; vp != NULL; vp = vp->GetNext ())
    {
        vp->Select ();
        vp->SetLastXY (vp->GetX (), vp->GetY ());
    }
}

void CShape::DeSelectAllVertex ()
{
    for (CVertex* vp = vertex_head; vp != NULL; vp = vp->GetNext ())
    {
        vp->DeSelect ();
    }
}

void CShape::Close ()
{
    ClosedFlag = true;
}

void CShape::Open ()
{
    ClosedFlag = false;
}

bool CShape::IsClosed ()
{
    return ClosedFlag;
}

void CShape::Select ()
{
    SelectedFlag = true;
    SelectAllVertex ();
}

void CShape::DeSelect ()
{
    SelectedFlag = false;
    DeSelectAllVertex ();
}

bool CShape::IsSelected ()
{
    return SelectedFlag;
}
