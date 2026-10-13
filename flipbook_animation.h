/*==============================================================================

   パラパラ漫画的アニメーション [flipbook_animation.h]
														 Author : Youhei Sato
														 Date   : 2026/06/09
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef FLIPBOOK_ANIMATION_H
#define FLIPBOOK_ANIMATION_H

#include <DirectXMath.h>


class FlipbookAnimation
{
private:
	int m_pattern_count{ 0 };          // パターン数（コマの総数）
	int m_pattern_columns{ 0 };        // 横に並ぶパターン数（画像の列数） 
	int m_pattern_width{0};            // パターン幅（1コマのピクセル幅） 
	int m_pattern_height{ 0 };         // パターン高さ（1コマのピクセル高さ） 
	float m_pattern_duration{ 0.0f };  // 1パターンの再生時間（秒）
	bool m_is_loop{ true };            // ループフラグ

	float m_elapsed_time{ 0.0f };      // アニメーション開始からの経過時間

	int m_texture_cut_x{ 0 };          // 計算された切り取り開始位置X 
	int m_texture_cut_y{ 0 };          // 計算された切り取り開始位置Y

public:
	FlipbookAnimation() = default;
	FlipbookAnimation(int pattern_count, int pattern_colums, int pattern_width, int pattern_height, float pattern_duration, bool is_loop = true)
		: m_pattern_count(pattern_count), m_pattern_columns(pattern_colums), m_pattern_width(pattern_width), m_pattern_height(pattern_height), m_pattern_duration(pattern_duration), m_is_loop(is_loop) {
	}
	void Update(float delta_time);

	/* 切り取り情報取得 */
	DirectX::XMFLOAT2 GetTextureCutPosition() const { return { (float)m_texture_cut_x, (float)m_texture_cut_y }; }
	DirectX::XMFLOAT2 GetTexureCutSize() const { return { (float)m_pattern_width, (float)m_pattern_height }; }
};

#endif // FLIPBOOK_ANIMATION_H