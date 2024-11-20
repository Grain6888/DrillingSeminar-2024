#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "WinOGL.h"
#endif

#include "WinOGLDoc.h"
#include "WinOGLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE (CWinOGLView, CView)

BEGIN_MESSAGE_MAP (CWinOGLView, CView)
    ON_WM_LBUTTONDOWN ()
    ON_WM_CREATE ()
    ON_WM_DESTROY ()
    ON_WM_ERASEBKGND ()
    ON_WM_SIZE ()
    ON_WM_MOUSEMOVE ()
    ON_WM_RBUTTONDOWN ()
    ON_COMMAND (ID_SIZEUP, &CWinOGLView::OnSizeUp)
    ON_COMMAND (ID_SIZEDOWN, &CWinOGLView::OnSizeDown)
    ON_COMMAND (ID_AXIS, &CWinOGLView::OnAxis)
    ON_UPDATE_COMMAND_UI (ID_AXIS, &CWinOGLView::OnUpdateAxis)
    ON_COMMAND (ID_EDITMODE, &CWinOGLView::OnEditMode)
    ON_UPDATE_COMMAND_UI (ID_EDITMODE, &CWinOGLView::OnUpdateEditMode)
    ON_WM_LBUTTONUP ()
    ON_COMMAND (ID_EDIT_UNDO, &CWinOGLView::OnEditUndo)
    ON_COMMAND (ID_DELETE_ALL, &CWinOGLView::OnDeleteAll)
END_MESSAGE_MAP ()

CWinOGLView::CWinOGLView () noexcept
{
    x_Ldown = 0.0;
    y_Ldown = 0.0;
    x_over = 0.0;
    y_over = 0.0;
    m_hRC = NULL;
}

CWinOGLView::~CWinOGLView ()
{ }

BOOL CWinOGLView::PreCreateWindow (CREATESTRUCT& cs)
{
    // TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
    //  修正してください。

    return CView::PreCreateWindow (cs);
}

// CWinOGLView 描画
void CWinOGLView::OnDraw (CDC* pDC)
{
    CWinOGLDoc* pDoc = GetDocument ();
    ASSERT_VALID (pDoc);
    if (!pDoc)
        return;

    wglMakeCurrent (pDC->m_hDC, m_hRC);
    glClearColor (0.95f, 0.95f, 0.95f, 1.00f);
    glClear (GL_COLOR_BUFFER_BIT /* | GL_DEPTH_BUFFER_BIT*/);

    AC.Draw (x_over, y_over);

    glFlush ();
    SwapBuffers (pDC->m_hDC);
    wglMakeCurrent (pDC->m_hDC, NULL);
}

// CWinOGLView の診断
#ifdef _DEBUG
void CWinOGLView::AssertValid () const
{
    CView::AssertValid ();
}

void CWinOGLView::Dump (CDumpContext& dc) const
{
    CView::Dump (dc);
}

CWinOGLDoc* CWinOGLView::GetDocument () const // デバッグ以外のバージョンはインラインです。
{
    ASSERT (m_pDocument->IsKindOf (RUNTIME_CLASS (CWinOGLDoc)));
    return (CWinOGLDoc*)m_pDocument;
}
#endif //_DEBUG

void CWinOGLView::OnLButtonDown (UINT nFlags, CPoint point)
{
    SetLDown (point);

    if (AC.IsEditMode () && !(nFlags & MK_SHIFT))
    {
        AC.DeSelectAllShape ();
        AC.AddVertex (x_Ldown, y_Ldown);
    }
    else if (AC.IsEditMode () && (nFlags & MK_SHIFT))
    {
        AC.SelectShapeElements (x_Ldown, y_Ldown, nFlags);
    }
    else
    {
        AC.SelectShapeElements (x_Ldown, y_Ldown, nFlags);
    }

    RedrawWindow ();

    CView::OnLButtonDown (nFlags, point);
}

void CWinOGLView::OnLButtonUp (UINT nFlags, CPoint point)
{
    if (!AC.IsEditMode ())
    {
        if (AC.IsInvalidMovedVertex ())
        {
            AC.ResetMovedVertex ();
        }
    }

    RedrawWindow ();

    CView::OnLButtonUp (nFlags, point);
}

void CWinOGLView::OnRButtonDown (UINT nFlags, CPoint point)
{
    RedrawWindow ();

    CView::OnRButtonDown (nFlags, point);
}

void CWinOGLView::OnMouseMove (UINT nFlags, CPoint point)
{
    SetOver (point);
    if (!AC.IsEditMode () && (nFlags & MK_LBUTTON))
    {
        if (IsMouseInside ())
        {
            AC.TrackVertexToMouse (x_over, y_over);
        }
        else
        {
            AC.ResetMovedVertex ();
        }
    }

    RedrawWindow ();

    CView::OnMouseMove (nFlags, point);
}

int CWinOGLView::OnCreate (LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate (lpCreateStruct) == -1)
        return -1;

    PIXELFORMATDESCRIPTOR pfd =
    {
    sizeof (PIXELFORMATDESCRIPTOR),
    1,
    PFD_DRAW_TO_WINDOW |
    PFD_SUPPORT_OPENGL |
    PFD_DOUBLEBUFFER,
    PFD_TYPE_RGBA,
    32,
    0,0,0,0,0,0,
    0,0,0,0,0,0,0,
    24,
    0,0,
    PFD_MAIN_PLANE,
    0,
    0,0,0
    };
    CClientDC clientDC (this);
    int pixelFormat = ChoosePixelFormat (clientDC.m_hDC,
        &pfd);
    SetPixelFormat (clientDC.m_hDC, pixelFormat, &pfd);
    m_hRC = wglCreateContext (clientDC.m_hDC);

    return 0;
}

void CWinOGLView::OnDestroy ()
{
    CView::OnDestroy ();
    wglDeleteContext (m_hRC);
}

BOOL CWinOGLView::OnEraseBkgnd (CDC* pDC)
{
    return true;
}

void CWinOGLView::OnSize (UINT nType, int cx, int cy)
{
    CView::OnSize (nType, cx, cy);

    CClientDC clientDC (this);
    wglMakeCurrent (clientDC.m_hDC, m_hRC);
    glViewport (0, 0, cx, cy);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

    float aspect_ratio = 0.0;
    //ウィンドウが横長の場合
    if (cx > cy)
    {
        aspect_ratio = (float)cx / cy;
        glOrtho (-1.0 * aspect_ratio, 1.0 * aspect_ratio, -1.0, 1.0, -100.0, 100.0);
    }
    //ウィンドウが縦長の場合
    else
    {
        aspect_ratio = (float)cy / cx;
        glOrtho (-1.0, 1.0, -1.0 * aspect_ratio, 1.0 * aspect_ratio, -100.0, 100.0);
    }

    glMatrixMode (GL_MODELVIEW);
    RedrawWindow ();
    wglMakeCurrent (clientDC.m_hDC, NULL);
}

void CWinOGLView::OnSizeUp ()
{
    AC.DrawSizeUp ();
    RedrawWindow ();
}

void CWinOGLView::OnSizeDown ()
{
    AC.DrawSizeDown ();
    RedrawWindow ();
}

void CWinOGLView::OnAxis ()
{
    AC.SwitchAxis ();
    RedrawWindow ();
}

void CWinOGLView::OnUpdateAxis (CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck (AC.IsShowingAxis ());
}

void CWinOGLView::OnEditMode ()
{
    AC.SwitchEditMode ();
    RedrawWindow ();
}

void CWinOGLView::OnUpdateEditMode (CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck (AC.IsEditMode ());
}

void CWinOGLView::OnEditUndo ()
{
    if (AC.IsEditMode ())
    {
        AC.SubVertex ();
    }
    else
    {
        AC.ResetMovedVertex ();
    }

    RedrawWindow ();
}


void CWinOGLView::OnDeleteAll ()
{
    AC.DeleteAllShape ();

    RedrawWindow ();
}

void CWinOGLView::SetLDown (CPoint point)
{
    // 描画領域の大きさを取得
    CRect rect;
    GetClientRect (rect);

    // デバイス座標系
    x_Ldown = (float)point.x;
    y_Ldown = (float)point.y;

    // デバイス座標系→正規化座標系
    x_Ldown = x_Ldown / rect.Width ();
    y_Ldown = 1 - (y_Ldown / rect.Height ());

    // 正規化座標系→ワールド座標系
    float aspect_ratio = 0.0;
    // ウィンドウが横長の場合
    if (rect.Width () > rect.Height ())
    {
        aspect_ratio = (float)rect.Width () / rect.Height ();
        x_Ldown = (x_Ldown - (float)(1.0 - x_Ldown)) * aspect_ratio;
        y_Ldown -= (float)(1.0 - y_Ldown);
    }
    // ウィンドウが縦長の場合
    else
    {
        aspect_ratio = (float)rect.Height () / rect.Width ();
        x_Ldown = x_Ldown - (float)(1.0 - x_Ldown);
        y_Ldown = (y_Ldown - (float)(1.0 - y_Ldown)) * aspect_ratio;
    }
}

void CWinOGLView::SetOver (CPoint point)
{
    // 描画領域の大きさを取得
    CRect rect;
    GetClientRect (rect);

    // デバイス座標系
    x_over = (float)point.x;
    y_over = (float)point.y;

    // デバイス座標系→正規化座標系
    x_over = x_over / rect.Width ();
    y_over = 1 - (y_over / rect.Height ());

    // 正規化座標系→ワールド座標系
    float aspect_ratio = 0.0;
    if (rect.Width () > rect.Height ())
    {
        aspect_ratio = (float)rect.Width () / rect.Height ();
        x_over = (x_over - (float)(1.0 - x_over)) * aspect_ratio;
        x_over = x_over;
        y_over -= (float)(1.0 - y_over);
        y_over = y_over;
    }
    else
    {
        aspect_ratio = (float)rect.Height () / rect.Width ();
        x_over = x_over - (float)(1.0 - x_over);
        x_over = x_over;
        y_over = (y_over - (float)(1.0 - y_over)) * aspect_ratio;
        y_over = y_over;
    }
}

bool CWinOGLView::IsMouseInside ()
{

    // 描画領域の大きさを取得
    CRect rect;
    GetClientRect (rect);

    float left = 0.0f;
    float right = 0.0f;
    float top = 0.0f;
    float bottom = 0.0f;

    if (rect.Width () > rect.Height ())
    {
        left = -(float)rect.Width () / rect.Height () + 0.01f;
        right = (float)rect.Width () / rect.Height () - 0.01f;
        top = 0.99f;
        bottom = -0.99f;
    }
    else
    {
        left = -0.99f;
        right = 0.99f;
        top = (float)rect.Height () / rect.Width () - 0.01f;
        bottom = -(float)rect.Height () / rect.Width () + 0.01f;
    }

    if (x_over <= left || x_over >= right)
    {
        return false;
    }
    else if (y_over <= bottom || y_over >= top)
    {
        return false;
    }

    return true;
}
