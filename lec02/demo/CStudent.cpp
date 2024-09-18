#include "CStudent.h"
#include "stdio.h"

// コンストラクタ
CStudent::CStudent ()
{
    name = 'N';
    math = 0;
    japanese = 0;
    english = 0;
}

CStudent::CStudent (char n, int m, int j, int e)
{
    name = n;
    math = m;
    japanese = j;
    english = e;
}

// デストラクタ
CStudent::~CStudent ()
{

}

// 学生の名前を入力
void CStudent::SetName (char n)
{
    name = n;
}


// 数学の点数を入力
void CStudent::SetMath (int m)
{
    if (m >= 0 && m <= 100)
    {
        math = m;
    }
    else
    {
        printf("点数として不正な値です．0点を設定します．\n");
        math = 0;
    }
}


// 国語の点数を入力
void CStudent::SetJapanese (int j)
{
    if (j >= 0 && j <= 100)
    {
        japanese = j;
    }
    else
    {
        printf ("点数として不正な値です．0点を設定します．\n");
        japanese = 0;
    }
}


// 英語の点数を入力
void CStudent::SetEnglish (int e)
{
    if (e >= 0 && e <= 100)
    {
        english = e;
    }
    else
    {
        printf("点数として不正な値です．0点を設定します．\n");
        english = 0;
    }
}


// 学生の名前を取得
char CStudent::GetName ()
{
    return name;
}


// 数学の点数を取得
int CStudent::GetMath ()
{
    return math;
}


// 国語の点数を取得
int CStudent::GetJapanese ()
{
    return japanese;
}


// 英語の点数を取得
int CStudent::GetEnglish ()
{
    return english;
}


// テストの平均点を取得
float CStudent::CalcAverage ()
{
    return (float)(math + japanese + english) / 3;
}
