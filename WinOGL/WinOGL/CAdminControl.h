#pragma once
#include "pch.h"
#include <gl/GL.h>
#include "CShape.h"

/// @brief 図形リストの管理（追加・削除・描画）を行うクラス．
class CAdminControl {
public:
    CAdminControl ();
    ~CAdminControl ();

    /// @brief 図形の描画を行う．ここからさらに，DrawPoint / DrawLine で点 / 線を描画する．また，DrawExpectedPoint / DrawExpectedLine で予測点 / 線を描画する．
    /// @param new_x 現在のマウスポインタの X 座標．予測点 / 線に使う．
    /// @param new_y 現在のマウスポインタの Y 座標．予測点 / 線に使う．
    void Draw (float new_x, float new_y);

    /// @brief 点の描画を行う．
    /// @param vertex 点の X,Y 座標．
    void DrawPoint (CVertex* vertex);

    /// @brief 辺の描画を行う．
    /// @param start 辺の始点の X,Y 座標．
    /// @param end   辺の終点の X,Y 座標．
    void DrawStrip (CVertex* start, CVertex* end);

    /// @brief 閉じた図形（多角形）の描画を行う．なお，始点の座標（start）と終点の座標（end）はそれぞれ別である．
    /// @param start 図形の始点の X,Y 座標．
    /// @param end   図形の終点の X,Y 座標．
    void DrawLoop (CVertex* start, CVertex* end);

    /// @brief 予測点（現在のマウスポインタの位置）の描画を行う．
    /// @param new_x 現在のマウスポインタの X 座標．
    /// @param new_y 現在のマウスポインタの Y 座標．
    void DrawExpectedPoint (float new_x, float new_y);

    /// @brief 予測線の描画を行う．
    /// @param start 予測線の X,Y 座標．通常は，描画中の図形（shape_tail）の点リストの終点（vertex_tail）を指定する．
    /// @param end_x 現在のマウスポインタの X 座標．
    /// @param end_y 現在のマウスポインタの Y 座標．
    void DrawExpectedLine (CVertex* start, float end_x, float end_y);

    /// @brief 最新の Shape セル（shape_tail）の次に，新しい Shape セルを追加する．
    void AddShape ();

    /// @brief 最新の Shape セル（shape_tail）を削除する．
    void DeleteShape ();

    /// @brief 図形リストに含まれる Shape セルの個数を取得する．
    /// @return 図形リストに含まれる Shape セルの個数．
    int GetShapeNum ();

    /// @brief 最新の Shape セル（shape_tail）に含まれる点リストに，新しい Vertex セルを追加する．
    /// @param new_x 新しい Vertex セルの X 座標．
    /// @param new_y 新しい Vertex セルの Y 座標．
    void AddList (float new_x, float new_y);

    /// @brief 最新の Shape セル（shape_tail）に含まれる点リストの，最新の Vertex セルを削除する．
    void SubList ();

    /// @brief 点の描画サイズを大きくし，線の描画幅を太くする．
    void DrawSizeUp ();

    /// @brief 点の描画サイズを小さくし，線の描画幅を細くする．
    void DrawSizeDown ();

    /// @brief 座標軸を描画する．
    void DrawAxis ();

    /// @brief 座標軸の表示または非表示の状態を切り替える．
    void SwitchAxis ();

    /// @brief 座標軸の表示または非表示の状態を取得する．
    /// @return 表示中なら true，非表示中なら false．
    bool GetAxis ();

    /// @brief 編集モードまたは選択モードの状態を切り替える．
    void SwitchEditMode ();

    /// @brief 編集モードまたは選択モードの状態を取得する．
    /// @return 編集モードなら true，選択モードなら false．
    bool GetEditMode ();
private:
    /// @brief 点の描画サイズ．
    float POINTSIZE = 10.0;

    /// @brief 線の描画幅．
    float LINEWIDTH = 2.0;

    /// @brief 図形リストの先頭の Shape セルを指すポインタ．
    CShape* shape_head;

    /// @brief 図形リストの最新の Shape セルを指すポインタ．
    CShape* shape_tail;

    /// @brief 図形リストに含まれる Shape セルの個数．
    int shape_num;

    /// @brief 座標軸の表示または非表示の状態を管理するフラグ．表示なら true，非表示なら false．
    bool AxisFlag = false;

    /// @brief 編集モードまたは選択モードの状態を管理するフラグ．編集モードなら true，選択モードなら false．
    bool EditModeFlag = false;
};