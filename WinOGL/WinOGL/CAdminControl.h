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

    /// @brief 図形の輪郭線の描画を行う．
    /// @param start 図形の始点
    /// @param end   図形の終点
    void DrawShape (CShape* shape);

    /// @brief 予測点の描画を行う．
    /// @param mouse マウスの座標 (X,Y)
    void DrawExVertex (CVertex* mouse);

    /// @brief 予測線の描画を行う．
    /// @param start 予測線の始点
    /// @param end   予測線の終点
    void DrawExLine (CVertex* start, CVertex* end);

    /// @brief マウスの近くの図形要素を選択する．
    /// @param mouse_x マウスの X 座標
    /// @param mouse_y マウスの Y 座標
    void EditShapeElements (float mouse_x, float mouse_y, UINT nFlags);

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

    /// @brief !!! 作りかけの移動を禁止中 !!! 頂点をマウスに追従する．
    /// @param mouse_x マウスの X 座標
    /// @param mouse_y マウスの Y 座標
    void TrackVertexToMouse (float mouse_x, float mouse_y);

    /// @brief 頂点を移動前の位置に戻す．
    void ResetMovedVertex ();

    /// @brief 図形を追加する．
    void PushShape ();

    /// @brief 図形を削除する．
    void PopShape ();

    /// @brief すべての図形を削除する．
    void DeleteAllShape ();

    /// @brief 図形の数を取得する．
    /// @return 図形の数
    int GetShapeNum ();

    /// @brief 図形に基づいて頂点を追加する．
    /// @param new_x 頂点の X 座標
    /// @param new_y 頂点の Y 座標
    void AddVertex (float new_x, float new_y);

    /// @brief 図形に基づいて頂点を削除する．
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
    /// @return 表示中 true / 非表示中 false．
    bool IsShowingAxis ();

    /// @brief 追加モードまたは編集モードの状態を切り替える．
    void SwitchAddMode ();

    /// @brief 追加モードまたは編集モードの状態を取得する．
    /// @return 追加モード true / 編集モード false．
    bool IsAddMode ();

    /// @brief 頂点の追加 / 挿入が可能かを判定する．
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

    /// @brief 移動した頂点が不正でないかを判定する．
    /// @return 不正 true / 不正でない false
    bool IsInvalidMovedVertex ();

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

    /// @brief 座標軸の表示または非表示の状態フラグ（表示 true / 非表示 false）．
    bool AxisFlag = false;

    /// @brief 追加モードまたは編集モードの状態フラグ（追加モード true / 編集モード false）．
    bool AddModeFlag = true;
};