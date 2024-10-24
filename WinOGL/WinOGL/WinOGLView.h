
// WinOGLView.h : CWinOGLView クラスのインターフェイス

#pragma once
#include <gl/GL.h>
#include "CAdminControl.h"


class CWinOGLView : public CView {
protected: // シリアル化からのみ作成します。
    CWinOGLView () noexcept;
    DECLARE_DYNCREATE (CWinOGLView)

// 属性
public:
    CWinOGLDoc* GetDocument () const;

// 操作

// オーバーライド
public:
    virtual void OnDraw (CDC* pDC);  // このビューを描画するためにオーバーライドされます。
    virtual BOOL PreCreateWindow (CREATESTRUCT& cs);

// 実装
public:
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
    afx_msg void OnMouseMove (UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown (UINT nFlags, CPoint point);
    afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy ();
    afx_msg BOOL OnEraseBkgnd (CDC* pDC);
    afx_msg void OnSize (UINT nType, int cx, int cy);
    afx_msg void OnSizeup ();
    afx_msg void OnSizedown ();
private:
    /// @brief 左クリックした時の，X 座標．
    float x_Ldown;

    /// @brief 左クリックした時の，Y 座標．
    float y_Ldown;

    /// @brief マウスオーバーした時の，X 座標．
    float x_over;

    /// @brief マウスオーバーした時の，Y 座標．
    float y_over;

    CAdminControl AC;
    HGLRC m_hRC;

    /// @brief デバイス座標系からワールド座標系に座標を変換する．
    /// @param point デバイス座標系 X,Y 座標．
    void DeviceP2WorldP (CPoint point);
};

#ifndef _DEBUG  // WinOGLView.cpp のデバッグ バージョン
inline CWinOGLDoc* CWinOGLView::GetDocument () const
{
    return reinterpret_cast<CWinOGLDoc*>(m_pDocument);
}
#endif

