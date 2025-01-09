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
    ON_WM_LBUTTONUP ()
    ON_COMMAND (ID_EDIT_UNDO, &CWinOGLView::OnEditUndo)
    ON_COMMAND (ID_DELETE_ALL, &CWinOGLView::OnDeleteAll)
    ON_WM_LBUTTONDBLCLK ()
    ON_WM_SETCURSOR ()
    ON_COMMAND (ID_FREE_SHAPE_MODE, &CWinOGLView::OnFreeShapeMode)
    ON_UPDATE_COMMAND_UI (ID_FREE_SHAPE_MODE, &CWinOGLView::OnUpdateFreeShapeMode)
    ON_WM_MOUSEWHEEL ()
    ON_WM_MBUTTONDOWN ()
    ON_COMMAND (ID_DRAW_SURFACE, &CWinOGLView::OnDrawSurface)
    ON_UPDATE_COMMAND_UI (ID_DRAW_SURFACE, &CWinOGLView::OnUpdateDrawSurface)
    ON_COMMAND (ID_VIEWPORT_TRANS, &CWinOGLView::OnViewportTrans)
    ON_UPDATE_COMMAND_UI (ID_VIEWPORT_TRANS, &CWinOGLView::OnUpdateViewportTrans)
END_MESSAGE_MAP ()

CWinOGLView::CWinOGLView () noexcept
{
    x_LR_down = 0.0;
    y_LR_down = 0.0;
    x_LR_over = 0.0;
    y_LR_over = 0.0;
    x_M_down = 0.0;
    y_M_down = 0.0;
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

    if (AC.IsScaleMode () || AC.IsRotateMode ())
    {
        AC.Draw (x_M_down, y_M_down);
    }
    else
    {
        AC.Draw (x_LR_over, y_LR_over);
    }

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
    CVertex mouse (x_LR_down, y_LR_down, NULL, NULL);

    if (AC.IsFreeShapeMode ())
    {
        AC.DeSelectAllShape ();
        AC.PushVertex (x_LR_down, y_LR_down);
    }
    else
    {
        if (DraggingFlag)
        {
            if (AC.CanMoveVertex ())
            {
                AC.UpdateLastMovedVertex ();
            }
            else
            {
                AC.ResetMovedVertex ();
            }
            DraggingFlag = false;
        }
        else
        {
            // Ctrl を押しながら左クリックで複数選択
            if (AC.SelectHandle (&mouse) == NULL && !(nFlags & MK_CONTROL))
            {
                AC.DeSelectAllShape ();
                if (AC.SelectVertex (&mouse) != NULL)
                {
                }
                else if (AC.SelectLine (&mouse) != NULL)
                {
                }
                else if (AC.SelectShape (&mouse) != NULL)
                {
                }
                else
                {
                    AC.DestroyBoundingBox ();
                    AC.ClearAffineTransMode ();
                }
            }
            else
            {
                CVertex base_p;
                AC.AutoSetBasePoint (&base_p);
                x_M_down = base_p.GetX ();
                y_M_down = base_p.GetY ();
            }


        }
    }

    RedrawWindow ();
    CView::OnLButtonDown (nFlags, point);
}

void CWinOGLView::OnLButtonDblClk (UINT nFlags, CPoint point)
{
    SetDown (point);
    CVertex mouse (x_LR_down, y_LR_down, NULL, NULL);

    AC.DeSelectAllShape ();
    if (AC.IsFreeShapeMode ())
    {
    }
    else
    {
        if (DraggingFlag)
        {
        }
        else
        {
            if (AC.SelectVertex (&mouse) == NULL && AC.SelectLine (&mouse) != NULL)
            {
                AC.AddVertex (x_LR_down, y_LR_down);
            }
        }
    }
    AC.DeSelectAllShape ();

    RedrawWindow ();
    CView::OnLButtonDblClk (nFlags, point);
}

void CWinOGLView::OnLButtonUp (UINT nFlags, CPoint point)
{
    SetDown (point);
    CVertex mouse (x_LR_down, y_LR_down, NULL, NULL);

    if (AC.IsFreeShapeMode ())
    {
    }
    else
    {
        if (DraggingFlag)
        {
            if (AC.CanMoveVertex ())
            {
                AC.UpdateLastMovedVertex ();
            }
            else
            {
                AC.ResetMovedVertex ();
            }
        }
        else
        {
            if (AC.SelectVertex (&mouse) != NULL)
            {
            }
            else if (AC.SelectLine (&mouse) != NULL)
            {
            }
            else if (AC.SelectShape (&mouse) != NULL)
            {
                AC.SwitchAffineTransMode ();
            }
        }
    }

    DraggingFlag = false;

    RedrawWindow ();
    CView::OnLButtonUp (nFlags, point);
}

void CWinOGLView::OnRButtonDown (UINT nFlags, CPoint point)
{
    SetDown (point);
    CVertex mouse (x_LR_down, y_LR_down, NULL, NULL);

    if (AC.IsFreeShapeMode ())
    {
    }
    else
    {
        if (DraggingFlag)
        {
            if (AC.CanMoveVertex ())
            {
                AC.UpdateLastMovedVertex ();
            }
            else
            {
                AC.ResetMovedVertex ();
            }
            AC.DeSelectAllShape ();
            DraggingFlag = false;
        }
        else
        {
            AC.DeSelectAllShape ();
            if (AC.SelectVertex (&mouse) != NULL)
            {
                AC.SubVertex ();
            }
            else if (AC.SelectShape (&mouse) != NULL)
            {
                AC.RemoveShape ();
            }
        }
    }

    RedrawWindow ();
    CView::OnRButtonDown (nFlags, point);
}

void CWinOGLView::OnMouseMove (UINT nFlags, CPoint point)
{
    SetOver (point);
    if (nFlags & MK_LBUTTON)
    {
        DraggingFlag = true;
    }

    if (AC.IsFreeShapeMode ())
    {
    }
    else
    {
        if (DraggingFlag)
        {
            if (AC.IsScaleMode () && AC.IsHandleSelected ())
            {
                CVertex base_p (x_M_down, y_M_down, NULL, NULL);
                AC.ScaleShape (&base_p, x_LR_down, y_LR_down, x_LR_over, y_LR_over);
            }
            else if (AC.IsRotateMode () && AC.IsHandleSelected ())
            {
                CVertex base_p (x_M_down, y_M_down, NULL, NULL);
                AC.RotateShape (&base_p, x_LR_down, y_LR_down, x_LR_over, y_LR_over);
            }
            else
            {
                AC.ShiftVertex (x_LR_down, y_LR_down, x_LR_over, y_LR_over);
            }
        }
    }

    RedrawWindow ();
    CView::OnMouseMove (nFlags, point);
}

BOOL CWinOGLView::OnMouseWheel (UINT nFlags, short zDelta, CPoint pt)
{
    CVertex base_p (x_M_down, y_M_down, NULL, NULL);

    if (AC.IsScaleMode ())
    {
        if (zDelta > 0)
        {
            AC.ScaleUpShape (&base_p);
        }
        else
        {
            AC.ScaleDownShape (&base_p);
        }
    }
    else if (AC.IsRotateMode ())
    {
        if (zDelta > 0)
        {
            AC.RotateLeftShape (&base_p);
        }
        else
        {
            AC.RotateRightShape (&base_p);
        }
    }

    if (AC.CanMoveVertex ())
    {
        AC.UpdateLastMovedVertex ();
    }
    else
    {
        AC.ResetMovedVertex ();
    }

    RedrawWindow ();
    return CView::OnMouseWheel (nFlags, zDelta, pt);
}

void CWinOGLView::OnMButtonDown (UINT nFlags, CPoint point)
{
    SetMDown (point);

    RedrawWindow ();
    CView::OnMButtonDown (nFlags, point);
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

void CWinOGLView::OnFreeShapeMode ()
{
    if (!AC.IsViewportTrans ())
    {
        AC.SwitchFreeShapeMode ();
    }
    RedrawWindow ();
}

void CWinOGLView::OnUpdateFreeShapeMode (CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck (AC.IsFreeShapeMode ());
}

void CWinOGLView::OnEditUndo ()
{
    if (AC.IsFreeShapeMode ())
    {
        AC.PopVertex ();
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
    x_LR_down = (float)point.x;
    y_LR_down = (float)point.y;

    // デバイス座標系→正規化座標系
    x_LR_down = x_LR_down / rect.Width ();
    y_LR_down = 1 - (y_LR_down / rect.Height ());

    // 正規化座標系→ワールド座標系
    float aspect_ratio = 0.0;
    // ウィンドウが横長の場合
    if (rect.Width () > rect.Height ())
    {
        aspect_ratio = (float)rect.Width () / rect.Height ();
        x_LR_down = (x_LR_down - (float)(1.0 - x_LR_down)) * aspect_ratio;
        y_LR_down -= (float)(1.0 - y_LR_down);
    }
    // ウィンドウが縦長の場合
    else
    {
        aspect_ratio = (float)rect.Height () / rect.Width ();
        x_LR_down = x_LR_down - (float)(1.0 - x_LR_down);
        y_LR_down = (y_LR_down - (float)(1.0 - y_LR_down)) * aspect_ratio;
    }
}

void CWinOGLView::SetOver (CPoint point)
{
    // 描画領域の大きさを取得
    CRect rect;
    GetClientRect (rect);

    // デバイス座標系
    x_LR_over = (float)point.x;
    y_LR_over = (float)point.y;

    // デバイス座標系→正規化座標系
    x_LR_over = x_LR_over / rect.Width ();
    y_LR_over = 1 - (y_LR_over / rect.Height ());

    // 正規化座標系→ワールド座標系
    float aspect_ratio = 0.0;
    if (rect.Width () > rect.Height ())
    {
        aspect_ratio = (float)rect.Width () / rect.Height ();
        x_LR_over = (x_LR_over - (float)(1.0 - x_LR_over)) * aspect_ratio;
        y_LR_over -= (float)(1.0 - y_LR_over);
    }
    else
    {
        aspect_ratio = (float)rect.Height () / rect.Width ();
        x_LR_over = x_LR_over - (float)(1.0 - x_LR_over);
        y_LR_over = (y_LR_over - (float)(1.0 - y_LR_over)) * aspect_ratio;
    }
}

void CWinOGLView::SetMDown (CPoint point)
{
    // 描画領域の大きさを取得
    CRect rect;
    GetClientRect (rect);

    // デバイス座標系
    x_M_down = (float)point.x;
    y_M_down = (float)point.y;

    // デバイス座標系→正規化座標系
    x_M_down = x_M_down / rect.Width ();
    y_M_down = 1 - (y_M_down / rect.Height ());

    // 正規化座標系→ワールド座標系
    float aspect_ratio = 0.0;
    if (rect.Width () > rect.Height ())
    {
        aspect_ratio = (float)rect.Width () / rect.Height ();
        x_M_down = (x_M_down - (float)(1.0 - x_M_down)) * aspect_ratio;
        y_M_down -= (float)(1.0 - y_M_down);
    }
    else
    {
        aspect_ratio = (float)rect.Height () / rect.Width ();
        x_M_down = x_M_down - (float)(1.0 - x_M_down);
        y_M_down = (y_M_down - (float)(1.0 - y_M_down)) * aspect_ratio;
    }
}


BOOL CWinOGLView::OnSetCursor (CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (AC.IsFreeShapeMode ())
    {
        CVertex mouse_over (x_LR_over, y_LR_over, NULL, NULL);
        if (AC.CanAddVertex (&mouse_over))
        {
            ::SetCursor (AfxGetApp ()->LoadStandardCursor (IDC_CROSS));
        }
        else
        {
            ::SetCursor (AfxGetApp ()->LoadStandardCursor (IDC_NO));
        }
    }
    else
    {
        if (DraggingFlag)
        {
            if (AC.CanMoveVertex ())
            {
                ::SetCursor (AfxGetApp ()->LoadStandardCursor (IDC_SIZEALL));
            }
            else
            {
                ::SetCursor (AfxGetApp ()->LoadStandardCursor (IDC_NO));
            }
        }
        else
        {
            ::SetCursor (AfxGetApp ()->LoadStandardCursor (IDC_SIZEALL));
        }
    }

    return TRUE;
    return CView::OnSetCursor (pWnd, nHitTest, message);
}

void CWinOGLView::OnDrawSurface ()
{
    AC.SwitchDrawSurface ();
    RedrawWindow ();
}

void CWinOGLView::OnUpdateDrawSurface (CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck (AC.IsDrawingSurface ());
}


void CWinOGLView::OnViewportTrans ()
{
    AC.SwitchViewportTrans ();
    AC.ClearAddShapeMode ();
    AC.DeSelectAllShape ();
    AC.ClearAffineTransMode ();
    RedrawWindow ();
}


void CWinOGLView::OnUpdateViewportTrans (CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck (AC.IsViewportTrans ());
}
