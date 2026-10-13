#include "scene_title.h"

#include "direct3d.h"
#include "polygon.h"
#include "texture.h"
#include "fade.h"

#include "scene_game.h"

#include "input.h"

SceneTitle::SceneTitle()
{
}

SceneTitle::~SceneTitle()
{
	delete m_sampler_state_linear;
	delete m_depth_stencil; // デプスステンシルステートの解放
	delete m_alpha_blend; // アルファブレンドステートの解放

	Texture_Release(m_button_texture_id2);
	Texture_Release(m_button_texture_id1);
	Texture_Release(m_bg_texture_id);
}

void SceneTitle::Initialize()
{
	m_bg_texture_id = Texture_Load(L"asset/texture/title.png");
	m_button_texture_id1 = Texture_Load(L"asset/texture/button1.png");
	m_button_texture_id2 = Texture_Load(L"asset/texture/button2.png");

	m_alpha_blend = new AlphaBlendState(AlphaBlendState::BlendMode::Alpha); // アルファブレンドステートの生成
	m_depth_stencil = new DepthStencilState(DepthStencilState::DepthStencilMode::None); // デプスステンシルステートの生成
	m_sampler_state_linear = new SamplerState(D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT);

	m_alpha_blend->Bind();
	m_depth_stencil->Bind();
	m_sampler_state_linear->Bind();

	Fade_Start(false, 1.0f);
}

void SceneTitle::Finalize()
{
}

void SceneTitle::Update(float /*delta_time*/)
{
	if (!m_SceneChanging)
	{
		// Check input for change scene;
		if (Input_IsKeyTrigger(KK_U))
		{
			// Fade;
			Fade_Start(true, 1.0f);
			m_SceneChanging = true;

			// Change scene to SceneGame
			//Scene_Change(new SceneGame);
		}
	}
	else
	{
		if (!Fade_IsFade())
		{
			// Change scene to SceneGame
			Scene_Change(new SceneGame);
		}
	}
}

void SceneTitle::Draw() const
{
	Polygon_Draw(
		m_bg_texture_id,
		0.0f,
		0.0f,
		Direct3D_GetBackBufferWidth(),
		Direct3D_GetBackBufferHeight(),
		0.0f,
		0.0f,
		Texture_Width(m_bg_texture_id),
		Texture_Height(m_bg_texture_id)
	);

	Polygon_Draw(
		m_button_texture_id2,
		(Direct3D_GetBackBufferWidth() / 2.0f) - (button_width / 2.0f),
		(Direct3D_GetBackBufferHeight() / 2.0f) - (button_height / 2.0f) + button_height_offset,
		button_width,
		button_height,
		0.0f,
		0.0f,
		Texture_Width(m_button_texture_id2),
		Texture_Height(m_button_texture_id2)
	);
}
