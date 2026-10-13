/*==============================================================================

   パラパラ漫画的アニメーション [flipbook_animation.cpp]
														 Author : Youhei Sato
														 Date   : 2026/06/09
--------------------------------------------------------------------------------

==============================================================================*/
#include "flipbook_animation.h"


void FlipbookAnimation::Update(float delta_time)
{
	// 経過時間を加算
	m_elapsed_time += delta_time;

	// 再生時間がアニメーションの総時間を超えた場合の処理
	float total_duration = m_pattern_duration * m_pattern_count; 
	
	if (m_elapsed_time > total_duration) {
		// ループする場合は時間リセット、ループしない場合は最終フレームで停止
		m_elapsed_time = m_is_loop ? 0.0f : total_duration;
	}

	// 現在のフレーム番号を算出 (経過時間 / 1フレーム時間)
	int current_pattern = static_cast<int>(m_elapsed_time / m_pattern_duration) % m_pattern_count;

	// 画像内の列数と幅を基準に、切り取り開始ピクセル座標を算出
	m_texture_cut_x = (current_pattern % m_pattern_columns) * m_pattern_width;
	m_texture_cut_y = (current_pattern / m_pattern_columns) * m_pattern_height;
}
