#pragma once
#include "pch.h"
#include "CVertex.h"

/// @brief Shape セルの中身と，それらに含まれる点リストの管理（追加・削除）を行うクラス．
class CShape {
public:
    CShape ();
    CShape (CShape* new_next, CShape* new_pre);
    ~CShape ();

    /// @brief 次の図形を設定する．
    /// @param new_next 次の図形
    void SetNext (CShape* new_next);

    /// @brief 前の図形を設定する．
    /// @param new_pre 前の図形
    void SetPre (CShape* new_pre);

    /// @brief 次の図形を取得する．
    /// @return 次の図形
    CShape* GetNext ();

    /// @brief 前の図形を取得する．
    /// @return 前の図形
    CShape* GetPre ();

    /// @brief 頂点の数を取得する．
    /// @return 頂点の数
    int GetVertexNum ();

    /// @brief 先頭の頂点を取得する．
    /// @return 先頭の頂点
    CVertex* CShape::GetHead ();

    /// @brief 末尾の頂点を取得する．
    /// @return 末尾の頂点
    CVertex* CShape::GetTail ();

    /// @brief 以降の図形を削除する．
    void FreeShape ();

    /// @brief 頂点を追加する．
    /// @param new_x 頂点の X 座標
    /// @param new_y 頂点の Y 座標
    void AddVertex (float new_x, float new_y);

    /// @brief 頂点を削除する．
    void DeleteVertex ();

    /// @brief 新しい頂点が自交差していないかを判定する．
    /// @param new_vertex 新しい頂点
    /// @return 自交差する true / 自交差しない false
    bool IsNewVertexSelfCross (CVertex* new_vertex);

    /// @brief 移動した頂点が自交差していないかを判定する．
    /// @param moved_vertex 移動した頂点
    /// @return 自交差する true / 自交差しない false
    bool IsMovedVertexSelfCross (CVertex* moved_vertex);

    /// @brief すべての頂点を選択済みにする．
    void SelectAllVertex ();

    /// @brief すべての頂点を未選択にする．
    void DeSelectAllVertex ();

    /// @brief 図形を選択済みにする．
    void Select ();

    /// @brief 図形を未選択にする．
    void DeSelect ();

    /// @brief 図形の選択状態を取得する．
    /// @return 選択済み true / 未選択 false
    bool IsSelected ();

private:
    /// @brief 先頭の頂点．
    CVertex* vertex_head;

    /// @brief 末尾の頂点．
    CVertex* vertex_tail;

    /// @brief 次の頂点．
    CShape* pre_shape;

    /// @brief 前の頂点．
    CShape* next_shape;

    /// @brief 頂点の数．
    int vertex_num;

    /// @brief 選択状態のフラグ．
    bool SelectedFlag = false;
};

