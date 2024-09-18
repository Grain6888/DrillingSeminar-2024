#pragma once
#include <stdio.h> 

class CVertex {
public:
    CVertex ();
    CVertex (int new_x, int new_y, CVertex* new_next);
    ~CVertex ();

private:
    // 頂点のX座標
    int x;
    // 頂点のY座標
    int y;
    // 次の頂点リストを指すポインタ
    CVertex* next_vertex;

public:
    // 頂点のXY座標を書き込む
    void SetXY (int new_x, int new_y);
    // 頂点のX座標を読み込む
    int GetX ();
    // 頂点のY座標を読み込む
    int GetY ();

    // 次の頂点リストを指すポインタを書き込む
    void SetNext (CVertex* new_next);
    // 次の頂点リストを指すポインタを読み込む
    CVertex* GetNext ();

    // リストを解放する
    void FreeVertex ();
};