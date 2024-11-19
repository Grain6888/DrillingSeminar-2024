#pragma once
#include "pch.h"
#include <gl/GL.h>
#include "CShape.h"

/// @brief 図形リストの管理（追加・削除・描画）を行うクラス．
class CAdminControl {
public:
    CAdminControl ();
    ~CAdminControl ();

    /// @brief すべての描画を行う．
    /// @param new_x マウスの X 座標
    /// @param new_y マウスの Y 座標
    void Draw (float new_x, float new_y);

    /// @brief 頂点の描画を行う．
    /// @param vertex 頂点
    void DrawVertex (CVertex* vertex);

    /// @brief 辺の描画を行う．
    /// @param start 辺の始点
    /// @param end   辺の終点
    void DrawLine (CVertex* start, CVertex* end);

    /// @brief 図形の描画を行う．
    /// @param start 図形の始点
    /// @param end   図形の終点
    void DrawShape (CShape* shape);

    /// @brief 予測点の描画を行う．
    /// @param mouse マウスの座標 (X,Y)
    void DrawMouseVertex (CVertex* mouse);

    /// @brief 予測線の描画を行う．
    /// @param start 予測線の始点
    /// @param end   予測線の終点
    void DrawMouseLine (CVertex* start, CVertex* end);

    /// @brief マウスの近くの要素を選択する．
    /// @param mouse_x マウスの X 座標
    /// @param mouse_y マウスの Y 座標
    void SelectShapeElements (float mouse_x, float mouse_y);

    /// @brief マウスの近くの頂点を探す．
    /// @param mouse マウスの座標 (X,Y)
    /// @return 見つかれば頂点アドレス，見つからなければ NULL．
    CVertex* SelectNearestVertex (CVertex* mouse);

    /// @brief マウスの近くの辺を探す．
    /// @param mouse マウスの座標 (X,Y)
    /// @return 見つかれば辺の始点のアドレス，見つからなければ NULL．
    CVertex* SelectNearestLine (CVertex* mouse);

    /// @brief マウスの近くの図形を探す．
    /// @param mouse マウスの座標 (X,Y)
    /// @return 見つかれば図形のアドレス，見つからなければ NULL．
    CShape* SelectNearestShape (CVertex* mouse);

    /// @brief 頂点をマウスに追従する．
    /// @param mouse_x マウスの X 座標
    /// @param mouse_y マウスの Y 座標
    void TrackVertexToMouse (float mouse_x, float mouse_y);

    /// @brief 頂点を移動前の位置に戻す．
    void ResetMovedVertex ();

    /// @brief 図形を追加する．
    void AddShape ();

    /// @brief 図形を削除する．
    void DeleteShape ();

    /// @brief 図形の数を取得する．
    /// @return 図形の数
    int GetShapeNum ();

    /// @brief 図形の頂点リストの末尾に頂点を追加する．
    /// @param new_x 頂点の X 座標
    /// @param new_y 頂点の Y 座標
    void PushVertex (float new_x, float new_y);

    /// @brief 図形の頂点リストの末尾の頂点を削除する．
    void PopVertex ();

    /// @brief 描画サイズを上げる．
    void DrawSizeUp ();

    /// @brief 描画サイズを下げる．
    void DrawSizeDown ();

    /// @brief 座標軸を描画する．
    void DrawAxis ();

    /// @brief 座標軸の表示または非表示の状態を切り替える．
    void SwitchAxis ();

    /// @brief 座標軸の表示または非表示の状態を取得する．
    /// @return 表示中なら true，非表示中なら false．
    bool IsShowingAxis ();

    /// @brief 編集モードまたは選択モードの状態を切り替える．
    void SwitchEditMode ();

    /// @brief 編集モードまたは選択モードの状態を取得する．
    /// @return 編集モードなら true，選択モードなら false．
    bool IsEditMode ();

    /// @brief 新しい頂点がほかの多角形に内包されていないかを判定する．
    /// @param new_vertex 新しい頂点
    /// @return 内包される場合は true，内包されない場合は false．
    bool IsNewVertexContained (CVertex* new_vertex);

    /// @brief 新しい多角形がほかの多角形を内包していないかを判定する．
    /// @return 内包する場合は true，内包しない場合は false．
    bool IsNewShapeContaining ();

    /// @brief 新しい頂点がほかの多角形と他交差していないかを判定する．
    /// @param new_vertex 新しい頂点
    /// @return 他交差する場合は true，他交差しない場合は false．
    bool IsNewVertexOtherCross (CVertex* new_vertex);

    /// @brief 移動した頂点が不正でないかを判定する．
    /// @return 不正な場合は true，不正でない場合は false．
    bool IsInvalidMovedVertex ();

    /// @brief 移動した頂点が多角形と他交差していないかを判定する．
    /// @param moved_vertex 移動した頂点
    /// @return 他交差する場合は true，他交差しない場合は false．
    bool IsMovedVertexOtherCross (CShape* my_shape, CVertex* moved_vertex);

    /// @brief 移動した頂点がほかの多角形に内包されていないかを判定する．
    /// @param moved_vertex 移動した頂点
    /// @return 内包される場合は true，内包されない場合は false．
    bool IsMovedVertexContained (CShape* my_shape, CVertex* moved_vertex);

    /// @brief 移動した多角形がほかの多角形を内包していないかを判定する．
    /// @param moved_shape 移動した多角形
    /// @return 内包する場合は true，内包しない場合は false．
    bool IsMovedShapeContaining (CShape* moved_shape);

    /// @brief すべての図形を選択状態にする．
    void SelectAllShape ();

    /// @brief すべての図形の選択状態を解除する．
    void DeSelectAllShape ();

private:
    /// @brief 点の描画サイズ．
    float POINTSIZE = 10.0;

    /// @brief 線の描画幅．
    float LINEWIDTH = 2.0;

    /// @brief 先頭の図形．
    CShape* shape_head;

    /// @brief 末尾の図形．
    CShape* shape_tail;

    /// @brief 図形の数．
    int shape_num;

    /// @brief 座標軸の表示または非表示の状態フラグ．表示なら true，非表示なら false．
    bool AxisFlag = false;

    /// @brief 編集モードまたは選択モードの状態フラグ．編集モードなら true，選択モードなら false．
    bool EditModeFlag = false;
};