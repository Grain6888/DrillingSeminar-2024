#pragma once
#include "pch.h"
#include "CVertex.h"
#include "CMath.h"

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
    CShape* pre_shape;
    // 次のリストを指すポインタ
    CShape* next_shape;

    // 形状リストのセルに含まれる点の数
    int vertex_num;

    CMath CM;

public:
    // 次のセルを指すポインタを書き込む
    void SetNext (CShape* new_next);
    // 前のセルを指すポインタを書き込む
    void SetPre (CShape* new_pre);

    // 次のセルを指すポインタを取得する
    CShape* GetNext ();
    // 前のセルを指すポインタを取得する
    CShape* GetPre ();
    // 形状リストのセルに含まれる点の数を取得する
    int GetVertexNum ();

    // 形状リストのセルに含まれる点リストの先頭を指すポインタを取得する
    CVertex* CShape::GetHead ();
    // 形状リストのセルに含まれる点リストの最後を指すポインタを取得する
    CVertex* CShape::GetTail ();

    // 形状リストを解放する
    void FreeShape ();

    // 点リストにセルを追加
    void AddVertex (float new_x, float new_y);
    // 点リストの最新の点を削除
    void DeleteVertex ();
};

