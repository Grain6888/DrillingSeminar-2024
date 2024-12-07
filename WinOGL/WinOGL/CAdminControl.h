#pragma once
#include "pch.h"
#include <gl/GL.h>
#include "CShape.h"

#define COLOR_WHITE 0.95f, 0.95f, 0.95f
#define COLOR_BLACK 0.15f, 0.15f, 0.15f
#define COLOR_PALE_BLUE 0.70f, 0.86f, 0.98f
#define COLOR_BLUE 0.30f, 0.40f, 0.78f
#define COLOR_LIGHT_BLUE 0.37f, 0.80f, 0.95f
#define COLOR_LIGHT_GREEN 0.65f, 0.91f, 0.32f
#define COLOR_GREEN 0.36f, 0.80f, 0.68f
#define COLOR_ORANGE 1.00f, 0.50f, 0.13f
#define COLOR_RED 0.94f, 0.25f, 0.14f

/// @brief 図形リストの管理（追加・削除・描画）を行うクラス．
class CAdminControl {
public:
    CAdminControl ();
    ~CAdminControl ();

    /// @brief すべての描画を行う．
    /// @param mouse_x マウスの X 座標
    /// @param mouse_y マウスの Y 座標
    void Draw (float mouse_x, float mouse_y);

    /// @brief 頂点の描画を行う．
    /// @param vertex 頂点
    void DrawVertex (CVertex* vertex);

    /// @brief 辺の描画を行う．
    /// @param start 辺の始点
    /// @param end   辺の終点
    void DrawLine (CVertex* start, CVertex* end);

    /// @brief 図形の輪郭線の描画を行う．
    /// @param start 図形の始点
    /// @param end   図形の終点
    void DrawOutline (CShape* shape);

    /// @brief 予測線の描画を行う．
    /// @param start 予測線の始点
    /// @param end   予測線の終点
    void DrawExLine (CVertex* start, CVertex* end);

    /// @brief 図形が選択された場合の補助線の描画を行う．
    void DrawNormalGuide ();

    /// @brief 拡大縮小モードの補助線と基点の描画を行う．
    /// @param base_p 基点
    void DrawScalingGuide (CVertex* base_p);

    /// @brief 回転モードの補助線と基点の描画を行う．
    /// @param base_p 基点
    void DrawRotatingGuide (CVertex* base_p);

    /// @brief マウスの近くの頂点を選択する．
    /// @param mouse マウスの座標 (X,Y)
    /// @return 選択した頂点を含む図形のアドレス / NULL
    CVertex* SelectVertex (CVertex* mouse);

    /// @brief マウスの近くの辺を選択する．
    /// @param mouse マウスの座標 (X,Y)
    /// @return 選択した辺を含む図形のアドレス / NULL
    CVertex* SelectLine (CVertex* mouse);

    /// @brief マウスの近くの図形を選択する．
    /// @param mouse マウスの座標 (X,Y)
    /// @return 選択した図形のアドレス / NULL
    CShape* SelectShape (CVertex* mouse);

    /// @brief 頂点を平行移動する．
    /// @param mouse_before_x 移動前のマウスの X 座標
    /// @param mouse_before_y 移動前のマウスの Y 座標
    /// @param mouse_after_x  移動後のマウスの X 座標
    /// @param mouse_after_y  移動後のマウスの Y 座標
    void ShiftVertex (float mouse_before_x, float mouse_before_y, float mouse_after_x, float mouse_after_y);

    /// @brief 形状を拡大縮小する．
    /// @param base_p         基点
    /// @param mouse_before_x 移動前のマウスの X 座標
    /// @param mouse_before_y 移動前のマウスの Y 座標
    /// @param mouse_after_x  移動後のマウスの X 座標
    /// @param mouse_after_y  移動後のマウスの Y 座標
    void ScaleShape (CVertex* base_p, float mouse_before_x, float mouse_before_y, float mouse_after_x, float mouse_after_y);

    /// @brief 形状を拡大する．
    /// @param base_p 基点
    void ScaleUpShape (CVertex* base_p);

    /// @brief 形状を縮小する．
    /// @param base_p 基点
    void ScaleDownShape (CVertex* base_p);

    /// @brief 形状を回転する．
    /// @param base_p         基点
    /// @param mouse_before_x 移動前のマウスの X 座標
    /// @param mouse_before_y 移動前のマウスの Y 座標
    /// @param mouse_after_x  移動後のマウスの X 座標
    /// @param mouse_after_y  移動後のマウスの Y 座標
    void RotateShape (CVertex* base_p, float mouse_before_x, float mouse_before_y, float mouse_after_x, float mouse_after_y);

    /// @brief 形状を左回転する．
    /// @param base_p 基点
    void RotateLeftShape (CVertex* base_p);

    /// @brief 形状を右回転する．
    /// @param base_p 基点
    void RotateRightShape (CVertex* base_p);

    /// @brief 頂点を移動前の位置に戻す．
    void ResetMovedVertex ();

    /// @brief 移動した頂点の直前の位置を更新する．
    void UpdateLastMovedVertex ();

    /// @brief 末尾に図形を追加する．
    void PushShape ();

    /// @brief 末尾の図形を削除する．
    void PopShape ();

    /// @brief 図形を削除する．
    void RemoveShape ();

    /// @brief すべての図形を削除する．
    void DeleteAllShape ();

    /// @brief 図形の数を取得する．
    /// @return 図形の数
    int GetShapeNum ();

    /// @brief 図形に基づいて頂点を追加する．
    /// @param new_x 頂点の X 座標
    /// @param new_y 頂点の Y 座標
    void PushVertex (float new_x, float new_y);

    /// @brief 図形に基づいて頂点を削除する．
    void PopVertex ();

    /// @brief 辺上に頂点を追加する．
    /// @param mouse_x マウスの X 座標
    /// @param mouse_y マウスの Y 座標
    void AddVertex (float mouse_x, float mouse_y);

    /// @brief 辺上の頂点を削除する．
    void SubVertex ();

    /// @brief 描画サイズを上げる．
    void DrawSizeUp ();

    /// @brief 描画サイズを下げる．
    void DrawSizeDown ();

    /// @brief 座標軸を描画する．
    void DrawAxis ();

    /// @brief 座標軸の表示または非表示の状態を切り替える．
    void SwitchAxis ();

    /// @brief 座標軸の表示または非表示の状態を取得する．
    /// @return 表示中 true / 非表示中 false
    bool IsShowingAxis ();

    /// @brief 自由図形モードの状態を切り替える．
    void SwitchFreeShapeMode ();

    /// @brief 自由図形モードの状態を取得する．
    /// @return 自由図形モードである true / 自由図形モードでない false
    bool IsFreeShapeMode ();

    /// @brief アフィン変換のモードを変更する（拡大縮小 → 回転）．
    void SwitchAffineTransMode ();

    /// @brief アフィン変換のモードをすべて解除する．
    void ClearAffineTransMode ();

    /// @brief 拡大縮小モードの状態を取得する．
    /// @return 拡大縮小モードである true / 拡大縮小モードでない false
    bool IsScaleMode ();

    /// @brief 回転モードの状態を取得する．
    /// @return 回転モードである true / 回転モードでない false
    bool IsRotateMode ();

    /// @brief 頂点の追加が可能かを判定する．
    /// @param new_vertex 新しい頂点
    /// @return 追加可能 true / 追加不可 false
    bool CanAddVertex (CVertex* new_vertex);

    /// @brief 新しい頂点がほかの多角形に内包されていないかを判定する．
    /// @param new_vertex 新しい頂点
    /// @return 内包される true / 内包されない false
    bool IsNewVertexContained (CVertex* new_vertex);

    /// @brief 新しい多角形がほかの多角形を内包していないかを判定する．
    /// @return 内包する true / 内包しない false
    bool IsNewShapeContaining ();

    /// @brief 新しい頂点がほかの多角形と他交差していないかを判定する．
    /// @param new_vertex 新しい頂点
    /// @return 他交差する true / 他交差しない false
    bool IsNewVertexOtherCross (CVertex* new_vertex);

    /// @brief 頂点の移動が可能かを判定する．
    /// @return 移動可能 true / 移動不可 false
    bool CanMoveVertex ();

    /// @brief !!! 閉じてない図形を移動できないバグ有り !!! 移動した頂点が多角形と他交差していないかを判定する．
    /// @param my_shape     自図形
    /// @param moved_vertex 移動した頂点
    /// @return 他交差する true / 他交差しない false
    bool IsMovedVertexOtherCross (CShape* my_shape, CVertex* moved_vertex);

    /// @brief 移動した頂点がほかの多角形に内包されていないかを判定する．
    /// @param my_shape     自図形
    /// @param moved_vertex 移動した頂点
    /// @return 内包される true / 内包されない false
    bool IsMovedVertexContained (CShape* my_shape, CVertex* moved_vertex);

    /// @brief 移動した多角形がほかの多角形を内包していないかを判定する．
    /// @param moved_shape 移動した多角形
    /// @return 内包する true / 内包しない false
    bool IsMovedShapeContaining (CShape* moved_shape);

    /// @brief 頂点の削除が可能かを判定する．
    /// @return 削除可能 true / 削除不可 false
    bool CanRemoveVertex ();

    /// @brief 辺上の点を削除する場合に他交差しないかを判定する．
    /// @param my_shape      自図形
    /// @param remove_vertex 削除する頂点
    /// @return 他交差する true / 他交差しない false
    bool IsRemoveVertexOtherCross (CShape* my_shape, CVertex* remove_vertex);

    /// @brief 辺上の点を削除する場合に多角形がほかの多角形を内包していないかを判定する．
    /// @param my_shape      自図形
    /// @param remove_vertex 削除する頂点
    /// @return 内包する true / 内包しない false
    bool IsRemoveShapeContaining (CShape* my_shape, CVertex* remove_vertex);

    /// @brief すべての図形を選択状態にする．
    void SelectAllShape ();

    /// @brief すべての図形の選択状態を解除する．
    void DeSelectAllShape ();

    /// @brief バウンディングボックスを生成する．
    void CreateBoundingBox ();

    /// @brief バウンディングボックスを更新する．
    void UpdateBoundingBox ();

    /// @brief バウンディングボックスを破棄する．
    void DestroyBoundingBox ();

    /// @brief バウンディングボックスのハンドルを選択する．
    /// @param mouse マウスの座標 (X,Y)
    /// @return ハンドル
    CVertex* SelectHandle (CVertex* mouse);

    void AutoSetBasePoint (CVertex* result);

    /// @brief ハンドルが選択されているかを判定する．
    /// @return 選択済 true / 未選択 false
    bool IsHandleSelected ();

private:
    /// @brief 点の描画サイズ．
    float POINTSIZE = 10.0;

    /// @brief 線の描画幅．
    float LINEWIDTH = 2.0;

    /// @brief 先頭の図形．
    CShape* shape_head;

    /// @brief 末尾の図形．
    CShape* shape_tail;

    /// @brief バウンディングボックス
    CShape* bounding_box;

    /// @brief 図形の数．
    int shape_num;

    /// @brief 座標軸の表示または非表示の状態フラグ（表示 true / 非表示 false）．
    bool AxisFlag = false;

    /// @brief 自由図形モードの状態フラグ（自由図形モードである true / 自由図形モードでない false）．
    bool FreeShapeModeFlag = true;

    /// @brief 拡大縮小モードの状態フラグ（拡大縮小モードである true / 拡大縮小モードでない false）．
    bool ScalingModeFlag = false;

    /// @brief 回転モードの状態フラグ（回転モードである true / 回転モードでない false）．
    bool RotatingModeFlag = false;
};