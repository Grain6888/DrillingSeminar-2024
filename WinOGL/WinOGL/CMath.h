#pragma once
#include "CVertex.h"
#include "CShape.h"
#define _USE_MATH_DEFINES
#include <math.h>

class CMath {
public:
    /// @brief 2 点間の距離を計算する．
    /// @param p1 1 つ目の点の X,Y 座標．
    /// @param p2 2 つ目の点の X,Y 座標．
    /// @return 2 点間の距離．ただし，0 から 1 で正規化された値．
    static float VertexDistance (CVertex* p1, CVertex* p2);

    /// @brief 2 点間の距離を計算する．
    /// @param p1   1 つ目の点の X,Y 座標．
    /// @param p2_x 2 つ目の点の X 座標．
    /// @param p2_y 2 つ目の点の Y 座標．
    /// @return 2 点間の距離．ただし，0 から 1 で正規化された値．
    static float VertexDistance (CVertex* p1, float p2_x, float p2_y);

    static float StripDistance (float v_x, float v_y, CVertex* sp_s, CVertex* sp_e);

    /// @brief 自分の図形に含まれる 2 辺が交差していないかを判定する．主に「自分の Shape セルに含まれる辺」と「新しく追加する辺」の自交差の有無の判定に用いる．
    /// @param a_s 1 つ目の辺の始点の X,Y 座標．通常は，図形リストの最新の Shape セルに含まれる点リストの最新の Vertex セルを指定する．
    /// @param a_e 1 つ目の辺の終点の X,Y 座標．通常は，図形リストに追加する新しい Vertex セルを指定する．
    /// @param b_s 2 つ目の辺の始点の X,Y 座標．
    /// @param b_e 2 つ目の辺の終点の X,Y 座標．
    /// @return 自交差の有無．交差する場合は true，交差しない場合は false．
    static bool CrossDetect (CVertex* a_s, CVertex* a_e, CVertex* b_s, CVertex* b_e);

    /// @brief 自分の図形に含まれる 1 辺がほかの図形に含まれる 1 辺と交差していないかを判定する．主に「自分以外の Shape セルに含まれる辺」と「新しく追加する辺」の他交差の有無の判定に用いる．ただし，新しく追加する辺の始点は，自分の図形に含まれる点リストの最新の Vertex セル（vertex_tail）に限定される．
    /// @param a_s   判定を行う図形リストの始点を指すポインタ．
    /// @param a_e   判定を行う図形リストの終点を指すポインタ．
    /// @param new_x 自分の図形に含まれる 1 辺の終点の X 座標．
    /// @param new_y 自分の図形に含まれる 1 辺の終点の Y 座標．
    /// @return 他交差の有無．交差する場合は true，交差しない場合は false．
    static bool OtherCrossDetect (CShape* a_s, CShape* a_e, float new_x, float new_y);

    /// @brief 1 点が多角形に内包されていないかを判定する．主に「自分以外の Shape セルの多角形」と「新しく追加する点」の内包濃霧の判定に用いる．
    /// @param a_s   判定を行う図形リストの始点を指すポインタ．
    /// @param a_e   判定を行う図形リストの終点を指すポインタ．
    /// @param new_x 1 点の X 座標．
    /// @param new_y 1 点の Y 座標．
    /// @return 内包する場合は true，内包しない場合は false．
    static bool InclusionDetect (CShape* a_s, CShape* a_e, float new_x, float new_y);

    /// @brief 2 つのベクトルの内積を計算する．
    /// @param p_a_s 1 つ目のベクトルの始点．
    /// @param p_a_e 1 つ目のベクトルの終点．
    /// @param p_b_s 2 つ目のベクトルの始点．
    /// @param p_b_e 2 つ目のベクトルの終点．
    /// @return 2 つのベクトルの内積．
    static float InnerProduct (CVertex* p_a_s, CVertex* p_a_e, CVertex* p_b_s, CVertex* p_b_e);

    /// @brief 2 つのベクトルの外積を計算する．
    /// @param p_a_s 1 つ目のベクトルの始点．
    /// @param p_a_e 1 つ目のベクトルの終点．
    /// @param p_b_s 2 つ目のベクトルの始点．
    /// @param p_b_e 2 つ目のベクトルの終点．
    /// @return 2 つのベクトルの外積．
    static float OuterProduct (CVertex* p_a_s, CVertex* p_a_e, CVertex* p_b_s, CVertex* p_b_e);

    /// @brief 始点と終点の座標から，位置ベクトルを計算する．
    /// @param p_s 始点の X,Y 座標．
    /// @param p_e 終点の X,Y 座標．
    /// @return 位置ベクトル．
    static CVertex* CalcPointVector (CVertex* p_s, CVertex* p_e);

    /// @brief 2 つのベクトルのなす角度（ラジアン）を計算する．
    /// @param p_a_s 1 つ目のベクトルの始点．
    /// @param p_a_e 1 つ目のベクトルの終点．
    /// @param p_b_s 2 つ目のベクトルの始点．
    /// @param p_b_e 2 つ目のベクトルの終点．
    /// @return 2 つのベクトルのなす角度（-π～π）．
    static double CalcAngle (CVertex* p_a_s, CVertex* p_a_e, CVertex* p_b_s, CVertex* p_b_e);
};

