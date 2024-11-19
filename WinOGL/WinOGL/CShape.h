#pragma once
#include "pch.h"
#include "CVertex.h"

/// @brief Shape セルの中身と，それらに含まれる点リストの管理（追加・削除）を行うクラス．
class CShape {
public:
    CShape ();
    CShape (CShape* new_next, CShape* new_pre);
    ~CShape ();

    /// @brief 一つ新しい Shape セルを指すポインタを設定する．
    /// @param new_next 設定する Shape セルのポインタ．
    void SetNext (CShape* new_next);

    /// @brief 一つ古い Shape セルを指すポインタを設定する．
    /// @param new_pre 設定する Shape セルのポインタ．
    void SetPre (CShape* new_pre);

    /// @brief 一つ新しい Shape セルを指すポインタを取得する．
    /// @return 一つ新しい Shape セルのポインタ．
    CShape* GetNext ();

    /// @brief 一つ古い Shape セルを指すポインタを取得する．
    /// @return 一つ古い Shape セルのポインタ．
    CShape* GetPre ();

    /// @brief 点リストに含まれる Vertex セルの個数を取得する．
    /// @return 点リストに含まれる Vertex セルの個数．
    int GetVertexNum ();

    /// @brief 図形リストに含まれる点リストの，先頭の Vertex セルを指すポインタを取得する．
    /// @return 図形リストに含まれる点リストの，先頭の Vertex セルを指すポインタ．
    CVertex* CShape::GetHead ();

    /// @brief 図形リストに含まれる点リストの，最新の Vertex セルを指すポインタを取得する．
    /// @return 図形リストに含まれる点リストの，最新の Vertex セルを指すポインタ．
    CVertex* CShape::GetTail ();

    /// @brief 自分の Shape セル以降の図形リストのメモリ領域を開放する．
    void FreeShape ();

    /// @brief 図形リストに含まれる点リストの最新の Vertex セル（vertex_tail）の次に，新しい Vertex セルを追加する．
    /// @param new_x 一つ新しい Vertex セルの X 座標．
    /// @param new_y 一つ新しい Vertex セルの Y 座標．
    void AddVertex (float new_x, float new_y);

    /// @brief 図形リストに含まれる点リストの最新の Vertex セル（vertex_tail）を削除する．
    void DeleteVertex ();

    /// @brief 新しい頂点が自交差していないかを判定する．
    /// @param new_vertex 新しい頂点
    /// @return 自交差する場合は true，自交差しない場合は false．
    bool IsNewVertexSelfCross (CVertex* new_vertex);

    /// @brief 移動した頂点が自交差していないかを判定する．
    /// @param moved_vertex 移動した頂点
    /// @return 自交差する場合は true，自交差しない場合は false．
    bool IsMovedVertexSelfCross (CVertex* moved_vertex);

    /// @brief すべての頂点を選択状態にする．
    void SelectAllVertex ();

    /// @brief すべての頂点の選択状態を解除する．
    void DeSelectAllVertex ();

    /// @brief 図形を選択状態にする．
    void Select ();

    /// @brief 図形の選択状態を解除する．
    void DeSelect ();

    /// @brief 図形の選択状態を取得する．
    /// @return 選択されている場合は true，選択されていない場合は false．
    bool IsSelected ();

private:
    /// @brief 点リストの先頭の Vertex セルを指すポインタ．
    CVertex* vertex_head;

    /// @brief 点リストの最新の Vertex セルを指すポインタ．
    CVertex* vertex_tail;

    /// @brief 選択状態のフラグ．
    bool SelectedFlag;

    /// @brief 図形リストの Shape セルの，一つ古いセルを指すポインタ．
    CShape* pre_shape;

    /// @brief 図形リストの Shape セルの，一つ新しいセルを指すポインタ．
    CShape* next_shape;

    /// @brief 点リストに含まれる Vertex セルの個数．
    int vertex_num;
};

