#pragma once
#include "pch.h"
#include <gl/GL.h>
#include "CShape.h"

class CAdminControl {
public:
    CAdminControl ();
    ~CAdminControl ();
    void Draw ();

private:
    // 形状リストのヘッド
    CShape* shape_head;
    // 形状リストの最後
    CShape* shape_tail;

public:
    // 点の描画
    void DrawPoint (CVertex* vertex);
    // 線の描画
    void DrawLine (CVertex* start, CVertex* end);

    // 左クリックでリストに形状を追加
    void AddShape ();
    // 右クリックで最新の形状を削除
    void DeleteShape ();

    // 形状リストに点を追加する
    void AddList (float new_x, float new_y);
};