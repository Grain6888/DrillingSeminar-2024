#pragma once
#include "pch.h"

/// @brief 頂点の管理を行うクラス．
class CVertex {
public:
    CVertex ();
    CVertex (float new_x, float new_y, CVertex* new_next, CVertex* new_pre);
    ~CVertex ();

    /// @brief 頂点の X,Y 座標を設定する．
    /// @param new_x 頂点の X 座標．
    /// @param new_y 頂点の Y 座標．
    void SetXY (float new_x, float new_y);

    /// @brief 移動前の頂点の X,Y 座標を設定する．
    /// @param x 移動前の頂点の X 座標．
    /// @param y 移動前の頂点の Y 座標．
    void SetLastXY (float x, float y);

    /// @brief 頂点の X 座標を取得する．
    /// @return 頂点の X 座標．
    float GetX ();

    /// @brief 移動前の頂点の X 座標を取得する．
    /// @return 移動前の頂点の X 座標．
    float GetLastX ();

    /// @brief 頂点の Y 座標を取得する．
    /// @return 頂点の Y 座標．
    float GetY ();

    /// @brief 移動前の頂点の Y 座標を取得する．
    /// @return 移動前の頂点の Y 座標．
    float GetLastY ();

    /// @brief 次の頂点を指すポインタを設定する．
    /// @param new_next 次の頂点のアドレス．
    void SetNext (CVertex* new_next);

    /// @brief 前の頂点を指すポインタを設定する．
    /// @param new_pre 前の頂点のアドレス．
    void SetPre (CVertex* new_pre);

    /// @brief 次の頂点を指すポインタを取得する．
    /// @return 次の頂点のアドレス．
    CVertex* GetNext ();

    /// @brief 前の頂点を指すポインタを取得する．
    /// @return 前の頂点のアドレス．
    CVertex* GetPre ();

    /// @brief 自分の Vertex セル以降の点リストのメモリ領域を開放する．
    void FreeVertex ();

    /// @brief 頂点を選択状態にする．
    void SetSelection ();

    /// @brief 頂点の選択状態を解除する．
    void SetNotSelection ();

    /// @brief 頂点の選択状態を取得する．
    /// @return 選択されているなら true，選択されていないなら false．
    bool GetSelection ();

private:
    /// @brief 頂点の X 座標．
    float x;

    /// @brief 頂点の Y 座標．
    float y;

    /// @brief 移動前の頂点の X 座標．
    float last_x;

    /// @brief 移動前の頂点の Y 座標．
    float last_y;

    /// @brief 選択状態のフラグ．
    bool selected_flag;

    /// @brief 点リストの Vertex セルの，一つ古いセルを指すポインタ．
    CVertex* pre_vertex;

    /// @brief 点リストの Vertex セルの，一つ新しいセルを指すポインタ．
    CVertex* next_vertex;
};