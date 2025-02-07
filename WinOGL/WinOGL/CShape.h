#pragma once
#include "pch.h"
#include <gl/GL.h>
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

    /// @brief 末尾に頂点を追加する．
    /// @param new_x 頂点の X 座標
    /// @param new_y 頂点の Y 座標
    void PushVertex (GLfloat new_x, GLfloat new_y);

    /// @brief 辺上に頂点を追加する．
    /// @param pre_vertex  辺の始点
    /// @param new_x       頂点の X 座標
    /// @param new_y       頂点の Y 座標
    /// @param next_vertex 辺の終点
    void InsertVertex (CVertex* pre_vertex, GLfloat new_x, GLfloat new_y, CVertex* next_vertex);

    /// @brief 末尾の頂点を削除する．
    void PopVertex ();

    /// @brief 辺上の頂点を削除する．
    /// @param remove_vertex 頂点
    void RemoveVertex (CVertex* remove_vertex);

    /// @brief 新しい頂点が自交差していないかを判定する．
    /// @param new_vertex 新しい頂点
    /// @return 自交差する true / 自交差しない false
    bool IsNewVertexSelfCross (CVertex* new_vertex);

    /// @brief 移動した頂点が自交差していないかを判定する．
    /// @param moved_vertex 移動した頂点
    /// @return 自交差する true / 自交差しない false
    bool IsMovedVertexSelfCross (CVertex* moved_vertex);

    /// @brief 辺上の頂点を削除する場合に自交差しないかを判定する．
    /// @param remove_vertex 削除する頂点
    /// @return 自交差する true / 自交差しない false
    bool IsRemoveVertexSelfCross (CVertex* remove_vertex);

    /// @brief すべての頂点を選択済みにする．
    void SelectAllVertex ();

    /// @brief すべての頂点を未選択にする．
    void DeSelectAllVertex ();

    /// @brief 図形を閉じる．
    void Close ();

    /// @brief 図形を開く．
    void Open ();

    /// @brief 図形が閉じ状態を取得する．
    /// @return 閉じている true / 開いている false
    bool IsClosed ();

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

    /// @brief 閉じた図形のフラグ（閉じている true / 開いている false）．
    bool ClosedFlag = false;

    /// @brief 選択状態のフラグ（選択済み true / 未選択 false）．
    bool SelectedFlag = false;
};

