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
    afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
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
    afx_msg void OnEditMode ();
    afx_msg void OnUpdateEditMode (CCmdUI* pCmdUI);
    afx_msg void OnEditUndo ();
    afx_msg void OnDeleteAll ();

    /// @brief 左右クリックした時の座標 (X,Y) を設定する．
    /// @param point デバイス座標 (X,Y)
    void SetDown (CPoint point);

    /// @brief マウスオーバーした時の座標 (X,Y) を設定する．
    /// @param point デバイス座標 (X,Y)
    void SetOver (CPoint point);

    /// @brief マウスがビューポート内にあるかを判定する．
    /// @return 内 true / 外 false
    bool IsMouseInside ();

private:
    /// @brief 左右クリックした時の，X 座標．
    float x_down;

    /// @brief 左右クリックした時の，Y 座標．
    float y_down;

    /// @brief マウスオーバーした時の，X 座標．
    float x_over;

    /// @brief マウスオーバーした時の，Y 座標．
    float y_over;

    CAdminControl AC;
    HGLRC m_hRC;
};

#ifndef _DEBUG  // WinOGLView.cpp のデバッグ バージョン
inline CWinOGLDoc* CWinOGLView::GetDocument () const
{
    return reinterpret_cast<CWinOGLDoc*>(m_pDocument);
}
#endif

