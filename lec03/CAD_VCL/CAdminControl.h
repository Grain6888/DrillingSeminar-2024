#pragma once
#include "resource.h"
#include "CDraw.h"
#include "CVertex.h"

#define MAX_LOADSTRING 100

class CAdminControl {
public:
    CAdminControl (HINSTANCE hInstance);
    ~CAdminControl ();
private:
    // 描画クラス内の関数を使用するための変数
    CDraw draw;
    // 頂点リストのヘッド
    CVertex* vertex_head;
protected:
    // 現在のインターフェイス
    HINSTANCE hInst;
public:
    // ウィンドウの定義する関数
    void MyRegisterClass ();
    // ウィンドウの生成する関数
    BOOL CreateWindowClass (int);
    // タブやキー入力などのアクションを処理する関数
    LRESULT WndProc (HWND, UINT, WPARAM, LPARAM);

    /* 課題2 AddVertex関数の作成*/
    // ここに記入
    void AddVertex (int new_x, int new_y);

    /* 課題3 DeleteVertex関数の作成*/
    // ここに記入
    void DeleteVertex ();
};

