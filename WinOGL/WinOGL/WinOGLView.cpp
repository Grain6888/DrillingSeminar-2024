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
    ON_WM_LBUTTONDBLCLK ()
END_MESSAGE_MAP ()

CWinOGLView::CWinOGLView () noexcept
{
    x_down = 0.0;
    y_down = 0.0;
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
    SetDown (point);
    CVertex mouse (x_down, y_down, NULL, NULL);

    if (AC.IsAddMode ())
    {
        AC.DeSelectAllShape ();
        AC.AddVertex (x_down, y_down);
    }
    else if (!AC.IsAddMode () && !DraggingFlag)
    {
        // Ctrl を押しながら左クリックで複数選択
        if (!(nFlags & MK_CONTROL))
        {
            AC.DeSelectAllShape ();
        }

        AC.SelectVertex (&mouse);
        AC.SelectLine (&mouse);
        AC.SelectShape (&mouse);
        DraggingFlag = true;
    }
    else if (!AC.IsAddMode () && DraggingFlag)
    {
        if (!AC.IsAddMode () && AC.IsInvalidMovedVertex ())
        {
            AC.ResetMovedVertex ();
        }
        DraggingFlag = false;
    }

    RedrawWindow ();

    CView::OnLButtonDown (nFlags, point);
}

void CWinOGLView::OnLButtonDblClk (UINT nFlags, CPoint point)
{
    SetDown (point);
    CVertex mouse (x_down, y_down, NULL, NULL);

    if (!AC.IsAddMode () && !DraggingFlag && AC.SelectLine (&mouse) != NULL)
    {
        AC.EditShapeElements (x_down, y_down, nFlags);
    }

    RedrawWindow ();

    CView::OnLButtonDblClk (nFlags, point);
}

void CWinOGLView::OnLButtonUp (UINT nFlags, CPoint point)
{
    if (!AC.IsAddMode () && AC.IsInvalidMovedVertex ())
    {
        AC.ResetMovedVertex ();
    }
    DraggingFlag = false;

    RedrawWindow ();

    CView::OnLButtonUp (nFlags, point);
}

void CWinOGLView::OnRButtonDown (UINT nFlags, CPoint point)
{
    SetDown (point);
    if (!AC.IsAddMode ())
    {
        AC.EditShapeElements (x_down, y_down, nFlags);
    }
    RedrawWindow ();

    CView::OnRButtonDown (nFlags, point);
}

void CWinOGLView::OnMouseMove (UINT nFlags, CPoint point)
{
    SetOver (point);
    if (!AC.IsAddMode () && DraggingFlag)
    {
        AC.TrackVertexToMouse (x_over, y_over);
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
    AC.SwitchAddMode ();
    RedrawWindow ();
}

void CWinOGLView::OnUpdateEditMode (CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck (AC.IsAddMode ());
}

void CWinOGLView::OnEditUndo ()
{
    if (AC.IsAddMode ())
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

void CWinOGLView::SetDown (CPoint point)
{
    // 描画領域の大きさを取得
    CRect rect;
    GetClientRect (rect);

    // デバイス座標系
    x_down = (float)point.x;
    y_down = (float)point.y;

    // デバイス座標系→正規化座標系
    x_down = x_down / rect.Width ();
    y_down = 1 - (y_down / rect.Height ());

    // 正規化座標系→ワールド座標系
    float aspect_ratio = 0.0;
    // ウィンドウが横長の場合
    if (rect.Width () > rect.Height ())
    {
        aspect_ratio = (float)rect.Width () / rect.Height ();
        x_down = (x_down - (float)(1.0 - x_down)) * aspect_ratio;
        y_down -= (float)(1.0 - y_down);
    }
    // ウィンドウが縦長の場合
    else
    {
        aspect_ratio = (float)rect.Height () / rect.Width ();
        x_down = x_down - (float)(1.0 - x_down);
        y_down = (y_down - (float)(1.0 - y_down)) * aspect_ratio;
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
