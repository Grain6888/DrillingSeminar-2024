#pragma once
#include "pch.h"
#include "CVertex.h"

class CShape {
public:
    CShape ();
    CShape (CShape* new_next, CShape* new_pre);
    ~CShape ();

private:
    // 点リストのヘッド
    CVertex* vertex_head;
    // 点リストの最後
    CVertex* vertex_tail;

    // 前のリストを指すポインタ
    CShape* pre_vertex;
    // 次のリストを指すポインタ
    CShape* next_vertex;

public:
    // 次のセルを指すポインタを書き込む
    void SetNext (CShape* new_next);
    // 前のセルを指すポインタを書き込む
    void SetPre (CShape* new_pre);

    // 次のセルを指すポインタを取得する
    CShape* GetNext ();
    // 前のセルを指すポインタを取得する
    CShape* GetPre ();

    // 形状の vertex_head を指すポインタを取得する
    CVertex* CShape::GetHead ();
    // 形状の vertex_tail を指すポインタを取得する
    CVertex* CShape::GetTail ();

    // リストを解放する
    void FreeShape ();

    // 左クリックでリストに点を追加
    void AddVertex (float new_x, float new_y);
    // 右クリックで最新の点を削除
    void DeleteVertex ();
};

