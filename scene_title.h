#pragma once

#include "scene.h"

#include "alpha_blend_state.h"
#include "depth_stencil_state.h"
#include "sampler_state.h"

class SceneTitle : public Scene
{
private:
	AlphaBlendState* m_alpha_blend{};
	DepthStencilState* m_depth_stencil{};
	SamplerState* m_sampler_state_linear{};

	int m_bg_texture_id{ -1 };
	int m_button_texture_id1{ -1 };
	int m_button_texture_id2{ -1 };
	bool m_SceneChanging{ false };

	const float button_width = 350.0f;
	const float button_height = 130.0f;
	const float button_height_offset = 200.0f;

public:
	SceneTitle();
	~SceneTitle();

	void Initialize() override;
	void Finalize() override;
	void Update(float delta_time) override;
	void Draw() const override;
};