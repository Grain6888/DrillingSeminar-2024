#pragma once
class CVertex {
public:
    CVertex();
    CVertex(int n, float x, float y);
    ~CVertex();
private:
    // 頂点の番号
    int number;
    // X座標
    float X;
    // Y座標
    float Y;
public:
    // 頂点の番号を設定
    void SetNumber (int n);
    // X座標の値を設定
    void SetX (float x);
    // Y座標の値を設定
    void SetY (float y);
    // 頂点の番号を取得
    int GetNumber ();
    // X座標の値を取得
    float GetX ();
    // Y座標の値を取得
    float GetY ();
    // 頂点情報の出力
    void PrintVertex ();
};

