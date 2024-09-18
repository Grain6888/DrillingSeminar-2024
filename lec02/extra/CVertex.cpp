#include "CVertex.h"
#include <stdio.h>

CVertex::CVertex ()
{
    number = 1;
    X = 1;
    Y = 1;
}

CVertex::CVertex (int n, float x, float y)
{
    number = n;
    X = x;
    Y = y;
}

CVertex::~CVertex ()
{

}

// 頂点の番号を設定
void CVertex::SetNumber (int n)
{
    number = n;
}


// X座標の値を設定
void CVertex::SetX (float x)
{
    X = x;
}


// Y座標の値を設定
void CVertex::SetY (float y)
{
    Y = y;
}


// 頂点の番号を取得
int CVertex::GetNumber ()
{
    return number;
}


// X座標の値を取得
float CVertex::GetX ()
{
    return X;
}


// Y座標の値を取得
float CVertex::GetY ()
{
    return Y;
}


// 頂点情報の出力
void CVertex::PrintVertex ()
{
    printf("頂点 %d のX座標は %.2f，Y座標は %.2f です．\n", number, X, Y);
}
