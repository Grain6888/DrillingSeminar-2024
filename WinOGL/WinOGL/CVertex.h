#pragma once
#include "pch.h"

/// @brief Vertex セルの中身の管理を行うクラス．
class CVertex {
public:
    CVertex ();
    CVertex (float new_x, float new_y, CVertex* new_next, CVertex* new_pre);
    ~CVertex ();

    /// @brief Vertex セルに X,Y 座標を設定する．
    /// @param new_x 設定する Vertex セルの X 座標．
    /// @param new_y 設定する Vertex セルの Y 座標．
    void SetXY (float new_x, float new_y);

    /// @brief Vertex セルの X 座標を取得する．
    /// @return Vertex セルの X 座標．
    float GetX ();

    /// @brief Vertex セルの Y 座標を取得する．
    /// @return Vertex セルの Y 座標．
    float GetY ();

    /// @brief 一つ新しい Vertex セルを指すポインタを設定する．
    /// @param new_next 設定する Vertex セルのポインタ．
    void SetNext (CVertex* new_next);

    /// @brief 一つ古い Vertex セルを指すポインタを設定する．
    /// @param new_pre 設定する Vertex セルのポインタ．
    void SetPre (CVertex* new_pre);

    /// @brief 一つ新しい Vertex セルを指すポインタを取得する．
    /// @return 一つ新しい Vertex セルのポインタ．
    CVertex* GetNext ();

    /// @brief 一つ古い Vertex セルを指すポインタを取得する．
    /// @return 一つ古い Vertex セルのポインタ．
    CVertex* GetPre ();

    /// @brief 自分の Vertex セル以降の点リストのメモリ領域を開放する．
    void FreeVertex ();

private:
    /// @brief Vertex セルの X 座標．
    float x;

    /// @brief Vertex セルの Y 座標．
    float y;

    /// @brief 点リストの Vertex セルの，一つ古いセルを指すポインタ．
    CVertex* pre_vertex;

    /// @brief 点リストの Vertex セルの，一つ新しいセルを指すポインタ．
    CVertex* next_vertex;
};