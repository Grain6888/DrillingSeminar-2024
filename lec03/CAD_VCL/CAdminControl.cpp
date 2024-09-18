#include "framework.h"
#include "CAdminControl.h"

WCHAR szTitle[MAX_LOADSTRING];       // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING]; // メイン ウィンドウ クラス名

INT_PTR CALLBACK About (HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain (_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER (hPrevInstance);
    UNREFERENCED_PARAMETER (lpCmdLine);

    // グローバル文字列を初期化する
    LoadStringW (hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW (hInstance, IDC_CADVCL, szWindowClass, MAX_LOADSTRING);

    // アプリケーション初期化の実行
    CAdminControl InitInstance (hInstance);

    if (!InitInstance.CreateWindowClass (nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators (hInstance, MAKEINTRESOURCE (IDC_CADVCL));

    MSG msg;

    // メイン メッセージ ループ:
    while (GetMessage (&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator (msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage (&msg);
            DispatchMessage (&msg);
        }
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK MainWndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    static CAdminControl* app = 0;

    switch (message)
    {
        case WM_CREATE:
        {
            CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
            app = (CAdminControl*)cs->lpCreateParams;

            return 0;
        }
    }

    // Don't start processing messages until after WM_CREATE.
    if (app)
        return app->WndProc (hWnd, message, wParam, lParam);
    else
        return DefWindowProc (hWnd, message, wParam, lParam);
}

CAdminControl::CAdminControl (HINSTANCE hInstance)
{
    //初期化
    hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する
    MyRegisterClass ();

    draw = {};
    vertex_head = NULL;
}

CAdminControl::~CAdminControl ()
{ }


//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
void CAdminControl::MyRegisterClass ()
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof (WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = MainWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInst;
    wcex.hIcon = LoadIcon (hInst, MAKEINTRESOURCE (IDI_CADVCL));
    wcex.hCursor = LoadCursor (nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW (IDC_CADVCL);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon (wcex.hInstance, MAKEINTRESOURCE (IDI_SMALL));

    RegisterClassExW (&wcex);
}

//
//   関数: CreateWindowClass(HINSTANCE, int)
//
//   目的: メイン ウィンドウを作成します
//
//   コメント: この関数で、メイン プログラム ウィンドウを作成および表示します。
//
BOOL CAdminControl::CreateWindowClass (int nCmdShow)
{
    HWND hWnd = CreateWindowW (szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInst, this);

    // ウィンドサイズ
    SetWindowPos (hWnd, NULL, 0, 0, 640, 480, SWP_NOMOVE | SWP_NOZORDER);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow (hWnd, nCmdShow);
    UpdateWindow (hWnd);

    return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CAdminControl::WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    int x = 0;
    int y = 0;

    switch (message)
    {
        case WM_COMMAND:
        {
            int wmId = LOWORD (wParam);
            // 選択されたメニューの解析:
            switch (wmId)
            {
                case IDM_ABOUT:
                    DialogBox (hInst, MAKEINTRESOURCE (IDD_ABOUTBOX), hWnd, About);
                    break;
                case IDM_EXIT:
                    DestroyWindow (hWnd);
                    break;
                case IDM_DELETE: // 「編集」->「全削除」が押されたら呼び出される

                    //* 全ての頂点を削除
                    vertex_head->FreeVertex ();
                    vertex_head = NULL;

                    InvalidateRect (hWnd, NULL, TRUE); //領域無効化
                    UpdateWindow (hWnd);               //再描画命令
                    break;
                default:
                    return DefWindowProc (hWnd, message, wParam, lParam);
            }
        }
        break;
        case WM_LBUTTONDOWN: // マウスを左クリックすると呼び出される
            // ウィンドウの座標を取得
            x = LOWORD (lParam);
            y = HIWORD (lParam);

            /* 課題2 左クリックした点をリストに追加 */
            // ここに記入
            AddVertex (x, y);

            InvalidateRect (hWnd, NULL, TRUE); //領域無効化
            UpdateWindow (hWnd);               //再描画命令
            break;
        case WM_RBUTTONDOWN: // マウスを左クリックすると呼び出される
            x = LOWORD (lParam);
            y = HIWORD (lParam);

            /* 課題3 右クリックで最新の点を削除 */
            // ここに記入
            DeleteVertex ();

            InvalidateRect (hWnd, NULL, TRUE); //領域無効化
            UpdateWindow (hWnd);               //再描画命令
            break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint (hWnd, &ps);
            // HDC を使用する描画コード
            draw.Draw (hdc, vertex_head);

            EndPaint (hWnd, &ps);
        }
        break;
        // ウィンドウを閉じるときに呼ばれる
        case WM_DESTROY:
            vertex_head->FreeVertex ();
            PostQuitMessage (0);
            return 0;
    }

    return DefWindowProc (hWnd, message, wParam, lParam);;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER (lParam);
    switch (message)
    {
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            if (LOWORD (wParam) == IDOK || LOWORD (wParam) == IDCANCEL)
            {
                EndDialog (hDlg, LOWORD (wParam));
                return (INT_PTR)TRUE;
            }
            break;
    }
    return (INT_PTR)FALSE;
}

void CAdminControl::AddVertex (int new_x, int new_y)
{
    CVertex* new_v = new CVertex;
    new_v->SetXY (new_x, new_y);

    //開始点かつ終了点（リストが空）の場合
    if (vertex_head == NULL)
    {
        vertex_head = new_v;
    }
    //中間点または終了点（リストが空でない）の場合
    else
    {
        for (CVertex* vp = vertex_head; vp != NULL; vp = vp->GetNext ())
        {
            if (vp->GetNext () == NULL)
            {
                vp->SetNext (new_v);
                break;
            }
        }
    }

    return;
}

void CAdminControl::DeleteVertex ()
{
    CVertex* pre_vp = new CVertex;
    CVertex* latest_v = new CVertex;

    //開始点かつ終了点（リストが空）の場合
    if (vertex_head == NULL)
    {
        return;
    }
    //リストの要素が１つの場合
    else if (vertex_head->GetNext () == NULL)
    {
        vertex_head->FreeVertex ();
        vertex_head = NULL;
    }
    //中間点または終了点（リストが空でない）の場合
    else
    {
        for (CVertex* vp = vertex_head; vp != NULL; vp = vp->GetNext ())
        {
            if (vp->GetNext () == NULL)
            {
                pre_vp->SetNext (NULL);
                vp->FreeVertex ();
                return;
            }
            pre_vp = vp;
        }
    }
}
