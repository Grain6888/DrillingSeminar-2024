#pragma once
#include "pch.h"
#include <gl/GL.h>

/// @brief 頂点の管理を行うクラス．
class CVertex {
public:
    CVertex ();
    CVertex (GLfloat new_x, GLfloat new_y, CVertex* new_next, CVertex* new_pre);
    ~CVertex ();

    /// @brief 頂点の X,Y 座標を設定する．
    /// @param new_x 頂点の X 座標
    /// @param new_y 頂点の Y 座標
    void SetXY (GLfloat new_x, GLfloat new_y);

    /// @brief 移動前の頂点の X,Y 座標を設定する．
    /// @param x 移動前の頂点の X 座標
    /// @param y 移動前の頂点の Y 座標
    void SetLastXY (GLfloat x, GLfloat y);

    /// @brief 頂点の X 座標を取得する．
    /// @return 頂点の X 座標
    GLfloat GetX ();

    /// @brief 移動前の頂点の X 座標を取得する．
    /// @return 移動前の頂点の X 座標
    GLfloat GetLastX ();

    /// @brief 頂点の Y 座標を取得する．
    /// @return 頂点の Y 座標
    GLfloat GetY ();

    /// @brief 移動前の頂点の Y 座標を取得する．
    /// @return 移動前の頂点の Y 座標
    GLfloat GetLastY ();

    /// @brief 次の頂点を設定する．
    /// @param new_next 次の頂点
    void SetNext (CVertex* new_next);

    /// @brief 前の頂点を設定する．
    /// @param new_pre 前の頂点
    void SetPre (CVertex* new_pre);

    /// @brief 次の頂点を取得する．
    /// @return 次の頂点
    CVertex* GetNext ();

    /// @brief 前の頂点を取得する．
    /// @return 前の頂点
    CVertex* GetPre ();

    /// @brief 以降の頂点を削除する．
    void FreeVertex ();

    /// @brief 頂点を選択済みにする．
    void Select ();

    /// @brief 頂点を未選択にする．
    void DeSelect ();

    /// @brief 頂点の選択状態を取得する．
    /// @return 選択済み true / 未選択 false
    bool IsSelected ();

private:
    /// @brief 頂点の X 座標．
    GLfloat x;

    /// @brief 頂点の Y 座標．
    GLfloat y;

    /// @brief 移動前の頂点の X 座標．
    GLfloat last_x;

    /// @brief 移動前の頂点の Y 座標．
    GLfloat last_y;

    /// @brief 次の頂点．
    CVertex* next_vertex;

    /// @brief 前の頂点．
    CVertex* pre_vertex;

    /// @brief 選択状態のフラグ．
    bool SelectedFlag = false;
};