#pragma once
#include "pch.h"

class CVertex {
public:
    CVertex ();
    CVertex (float new_x, float new_y, CVertex* new_next, CVertex* new_pre);
    ~CVertex ();

private:
    // 頂点のX座標
    float x;
    // 頂点のY座標
    float y;

    // 点リストの前のセルを指すポインタ
    CVertex* pre_vertex;
    // 点リストの次のセルを指すポインタ
    CVertex* next_vertex;

public:
    // 頂点のXY座標を書き込む
    void SetXY (float new_x, float new_y);

    // 点リストのセルのX座標を取得する
    float GetX ();
    // 点リストのセルのY座標を取得する
    float GetY ();

    // 点リストの次のセルを指すポインタを書き込む
    void SetNext (CVertex* new_next);
    // 点リストの前のセルを指すポインタを書き込む
    void SetPre (CVertex* new_pre);

    // 点リストの次のセルを指すポインタを取得する
    CVertex* GetNext ();
    // 点リストの前のセルを指すポインタを取得する
    CVertex* GetPre ();

    // 点リストを解放する
    void FreeVertex ();
};