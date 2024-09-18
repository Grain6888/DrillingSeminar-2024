#include "CDraw.h"

// 描画メイン関数
void CDraw::Draw (HDC hdc, CVertex* vertex_head)
{
    if (vertex_head != NULL)
    {
        d_hdc = hdc;
        /* 課題1 リストの内容を描画する */
        // ここに記入
        for (CVertex* vp = vertex_head; vp != NULL; vp = vp->GetNext ())
        {
            DrawPoint (vp);

            //終了点の場合
            if (vp->GetNext () == NULL)
            {
                break;
            }
            //中間点の場合
            else
            {
                DrawLine (vp, vp->GetNext ());
            }
        }
    }

    return;
}

// 点の描画
void CDraw::DrawPoint (CVertex* vertex)
{
    // 点の大きさ
    int rad = 5;
    // 色
    COLORREF col = RGB (0, 0, 0);

    // 塗りつぶしの設定
    SelectObject (d_hdc, CreateSolidBrush (col));

    // 点の描画
    Ellipse (d_hdc, vertex->GetX () - rad, vertex->GetY () - rad, vertex->GetX () + rad, vertex->GetY () + rad);
}

// 線の描画
void CDraw::DrawLine (CVertex* start, CVertex* end)
{
    // 線の太さ
    int width = 2;
    // 色
    COLORREF col = RGB (0, 0, 0);

    // 太さと線の書式設定
    SelectObject (d_hdc, CreatePen (PS_SOLID, width, col));

    // 始点
    MoveToEx (d_hdc, start->GetX (), start->GetY (), NULL);

    // 終点
    LineTo (d_hdc, end->GetX (), end->GetY ());
}