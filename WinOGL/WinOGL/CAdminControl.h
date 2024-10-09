#pragma once
#include "pch.h"
#include <gl/GL.h>
#include "CVertex.h"

class CAdminControl {
public:
    CAdminControl ();
    ~CAdminControl ();
    void Draw ();
private:
    // 点リストのヘッド
    CVertex* vertex_head;
    // 点リストの最後
    CVertex* vertex_tail;
    // 点の描画
    void DrawPoint (CVertex* vertex);
    // 線の描画
    void DrawLine (CVertex* start, CVertex* end);
public:
    // 左クリックでリストに点を追加
    void AddVertex (float new_x, float new_y);
    // 右クリックで最新の点を削除
    void DeleteVertex ();
};