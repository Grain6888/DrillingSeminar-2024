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
    // ループの描画
    void DrawLoop (CVertex* start, CVertex* end);

    // 形状リストにセルを追加
    void AddShape ();
    // 形状リストの最新のセルを削除
    void DeleteShape ();

    // 形状リストに点を追加
    void AddList (float new_x, float new_y);
    // 形状リストに予測点を追加
    //void AddTmpList (float new_x, float new_y);
    // 形状リストから点を削除
    void SubList ();
};