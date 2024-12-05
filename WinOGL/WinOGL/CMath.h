#pragma once
#include "CVertex.h"
#include "CShape.h"
#define _USE_MATH_DEFINES
#include <math.h>

class CMath {
public:
    /// @brief 2 点間の距離を計算する．
    /// @param p1 1 つ目の頂点
    /// @param p2 2 つ目の頂点
    /// @return 2 点間の距離 (0~1)
    static float VertexDis (CVertex* p1, CVertex* p2);

    /// @brief 点と直線間の距離を計算する．
    /// @param p      頂点
    /// @param line_s 直線の始点
    /// @param line_e 直線の終点
    /// @return 点と直線間の距離 (0~1)
    static float LineDis (CVertex* p, CVertex* line_s, CVertex* line_e);

    /// @brief 点の垂線と直線間の交点を計算する．
    /// @param p      頂点
    /// @param line_s 直線の視点
    /// @param line_e 直線の終点
    /// @param result 交点
    /// @return 点の垂線と直線間の交点 (X,Y)
    static void CrossPoint (CVertex* p, CVertex* line_s, CVertex* line_e, CVertex* result);

    /// @brief マウスの移動量と方向に基づいて点を平行移動する．
    /// @param before 移動前のマウスの位置
    /// @param after  移動後のマウスの位置
    /// @param result 頂点
    static void ShiftPoint (CVertex* before, CVertex* after, CVertex* result);

    /// @brief マウスの移動量と方向に基づいて点を拡大 / 縮小移動する．
    /// @param before 移動前のマウスの位置
    /// @param after  移動後のマウスの位置
    /// @param result 頂点
    static void ScalePoint (CVertex* before, CVertex* after, CVertex* result);

    /// @brief 基点に基づいて点を拡大移動する．
    /// @param base_p 基点
    /// @param result 頂点
    static void ScaleUpPoint (CVertex* base_p, CVertex* result);

    /// @brief 基点に基づいて点を縮小移動する．
    /// @param base_p 基点
    /// @param result 頂点
    static void ScaleDownPoint (CVertex* base_p, CVertex* result);

    /// @brief 基点に基づいて点を回転する．
    /// @param degree  回転角度（0~360°）
    /// @param base_p 基点
    /// @param result 頂点
    static void RotatePoint (float degree, CVertex* base_p, CVertex* result);

    /// @brief 2 本の辺が交差していないかを判定する．
    /// @param a_s 1 本目の辺の始点
    /// @param a_e 1 本目の辺の終点
    /// @param b_s 2 本目の辺の始点
    /// @param b_e 2 本目の辺の終点
    /// @return 交差する true / 交差しない false
    static bool IsLineCrossing (CVertex* a_s, CVertex* a_e, CVertex* b_s, CVertex* b_e);

    /// @brief 多角形が点を内包していないかを判定する．
    /// @param my_shape   多角形
    /// @param new_vertex 頂点
    /// @return 内包する true / 内包しない false
    static bool IsContained (CShape* my_shape, CVertex* new_vertex);

    /// @brief 多角形が点を内包していないかを判定する．
    /// @param my_shape    多角形
    /// @param new_vertex  頂点
    /// @param skip_vertex 削除する頂点
    /// @return 内包する true / 内包しない false
    static bool IsContained (CShape* my_shape, CVertex* new_vertex, CVertex* skip_vertex);

    /// @brief 2 つのベクトルの内積を計算する．
    /// @param p_a_s 1 つ目のベクトルの始点
    /// @param p_a_e 1 つ目のベクトルの終点
    /// @param p_b_s 2 つ目のベクトルの始点
    /// @param p_b_e 2 つ目のベクトルの終点
    /// @return 2 つのベクトルの内積
    static float Inner (CVertex* p_a_s, CVertex* p_a_e, CVertex* p_b_s, CVertex* p_b_e);

    /// @brief 2 つのベクトルの外積を計算する．
    /// @param p_a_s 1 つ目のベクトルの始点
    /// @param p_a_e 1 つ目のベクトルの終点
    /// @param p_b_s 2 つ目のベクトルの始点
    /// @param p_b_e 2 つ目のベクトルの終点
    /// @return 2 つのベクトルの外積
    static float Outer (CVertex* p_a_s, CVertex* p_a_e, CVertex* p_b_s, CVertex* p_b_e);

    /// @brief 位置ベクトルを計算する．
    /// @param p_s 始点
    /// @param p_e 終点
    /// @return 位置ベクトル (X,Y)
    static CVertex* PositionVec (CVertex* p_s, CVertex* p_e);

    /// @brief 2 つのベクトルのなす角度（ラジアン）を計算する．
    /// @param p_a_s 1 つ目のベクトルの始点
    /// @param p_a_e 1 つ目のベクトルの終点
    /// @param p_b_s 2 つ目のベクトルの始点
    /// @param p_b_e 2 つ目のベクトルの終点
    /// @return 2 つのベクトルのなす角度 (-π~π)
    static double VecAngle (CVertex* p_a_s, CVertex* p_a_e, CVertex* p_b_s, CVertex* p_b_e);
};

