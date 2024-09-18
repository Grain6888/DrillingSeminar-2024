#pragma once
class CStudent
{
public:
	CStudent();
	CStudent(char n, int m, int j, int e);
	~CStudent();
private:
	// 学生の名前
	char name;
	// 数学の点数
	int math;
	// 国語の点数
	int japanese;
	// 英語の点数
	int english;
public:
	// 学生の名前を入力
	void SetName (char n);
	// 数学の点数を入力
	void SetMath (int m);
	// 国語の点数を入力
	void SetJapanese (int j);
	// 英語の点数を入力
	void SetEnglish (int e);
	// 学生の名前を取得
	char GetName ();
	// 数学の点数を取得
	int GetMath ();
	// 国語の点数を取得
	int GetJapanese ();
	// 英語の点数を取得
	int GetEnglish ();
	// テストの平均点を取得
	float CalcAverage ();
};