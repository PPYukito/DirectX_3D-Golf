#pragma once

#include "flipbook_animation.h"
#include "billboard.h"
#include "SimdMath.h"

class BillboardAnimation
{
private:
	FlipbookAnimation m_anim;
	int m_tex_id{ -1 };

public:
	BillboardAnimation(int texId, int count, int col, int width, int height, float duration, bool isLoop = true)
		: m_tex_id(texId), m_anim( count, col, width, height, duration, isLoop)
	{ }

	void Update(float delta_time) {
		m_anim.Update(delta_time);
	}

	void Draw(const Vector3& position, const Vector3& size) const
	{
		BillBoard_Draw(
			m_tex_id,
			position,
			size,
			m_anim.GetTextureCutPosition(),
			m_anim.GetTexureCutSize()
		);
	}
};