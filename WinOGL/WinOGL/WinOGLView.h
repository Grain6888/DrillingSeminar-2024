#pragma once
#include <gl/GL.h>
#include "CAdminControl.h"


class CWinOGLView : public CView {
protected: // シリアル化からのみ作成します。
    CWinOGLView () noexcept;
    DECLARE_DYNCREATE (CWinOGLView)

public:
    CWinOGLDoc* GetDocument () const;
    virtual void OnDraw (CDC* pDC);  // このビューを描画するためにオーバーライドされます。
    virtual BOOL PreCreateWindow (CREATESTRUCT& cs);
    virtual ~CWinOGLView ();

#ifdef _DEBUG
    virtual void AssertValid () const;
    virtual void Dump (CDumpContext& dc) const;
#endif

// 生成された、メッセージ割り当て関数
protected:
    DECLARE_MESSAGE_MAP ()
public:
    /// @brief 左右クリックした時の座標 (X,Y) を設定する．
    /// @param point デバイス座標 (X,Y)
    void SetDown (CPoint point);

    /// @brief マウスオーバーした時の座標 (X,Y) を設定する．
    /// @param point デバイス座標 (X,Y)
    void SetOver (CPoint point);

    /// @brief ホイールクリックしたときの座標 (X,Y) を設定する．
    /// @param point デバイス座標 (X,Y)
    void SetMDown (CPoint point);

private:
    /// @brief 左右クリックした時の X 座標．
    float x_LR_down;

    /// @brief 左右クリックした時の Y 座標．
    float y_LR_down;

    /// @brief マウスオーバーした時の X 座標．
    float x_LR_over;

    /// @brief マウスオーバーした時の Y 座標．
    float y_LR_over;

    /// @brief ホイールクリックしたときの X 座標．
    float x_M_down;

    /// @brief ホイールクリックしたときの Y 座標．
    float y_M_down;

    /// @brief D&D 中の状態フラグ（D&D 中である true / D&D 中でない false）．
    bool DraggingFlag = false;

    CAdminControl AC;
    HGLRC m_hRC;
public:
    afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk (UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp (UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown (UINT nFlags, CPoint point);
    afx_msg void OnMouseMove (UINT nFlags, CPoint point);
    afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy ();
    afx_msg BOOL OnEraseBkgnd (CDC* pDC);
    afx_msg void OnSize (UINT nType, int cx, int cy);
    afx_msg void OnSizeUp ();
    afx_msg void OnSizeDown ();
    afx_msg void OnAxis ();
    afx_msg void OnUpdateAxis (CCmdUI* pCmdUI);
    afx_msg void OnFreeShapeMode ();
    afx_msg void OnUpdateFreeShapeMode (CCmdUI* pCmdUI);
    afx_msg void OnEditUndo ();
    afx_msg void OnDeleteAll ();
    afx_msg BOOL OnSetCursor (CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg BOOL OnMouseWheel (UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnMButtonDown (UINT nFlags, CPoint point);
    afx_msg void OnDrawSurface ();
    afx_msg void OnUpdateDrawSurface (CCmdUI* pCmdUI);
    afx_msg void OnViewportTrans ();
    afx_msg void OnUpdateViewportTrans (CCmdUI* pCmdUI);
};

#ifndef _DEBUG  // WinOGLView.cpp のデバッグ バージョン
inline CWinOGLDoc* CWinOGLView::GetDocument () const
{
    return reinterpret_cast<CWinOGLDoc*>(m_pDocument);
}
#endif

