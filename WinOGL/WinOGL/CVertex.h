#pragma once
#include "pch.h"

class CVertex {
public:
    CVertex ();
    CVertex (float new_x, float new_y, CVertex* new_next, CVertex* new_pre);
    ~CVertex ();
private:
    // x座標
    float x;
    // y座標
    float y;
    // 前のリストを指すポインタ
    CVertex* pre_vertex;
    // 次のリストを指すポインタ
    CVertex* next_vertex;

public:
    // 頂点のxy座標を書き込む
    void SetXY (float new_x, float new_y);

    // x座標をセルから取得する
    float GetX ();
    // y座標をセルから取得する
    float GetY ();

    // 次のセルを指すポインタを書き込む
    void SetNext (CVertex* new_next);
    // 前のセルを指すポインタを書き込む
    void SetPre (CVertex* new_pre);

    // 次のセルを指すポインタを取得する
    CVertex* GetNext ();
    // 前のセルを指すポインタを取得する
    CVertex* GetPre ();

    // リストを解放する
    void FreeVertex ();
};

