#include "scene.h"

#include "scene_title.h"
#include "scene_game.h"

static Scene* m_pCurrentScene{ nullptr };
static Scene* m_pNextScene{ m_pCurrentScene };

void Scene_Initialize()
{
	//m_pCurrentScene->Initialize();
	m_pNextScene = m_pCurrentScene = new SceneTitle;
	m_pCurrentScene->Initialize();

}

void Scene_Finalize()
{
	//m_pCurrentScene->Finalize();
	delete m_pCurrentScene;
	m_pCurrentScene = m_pNextScene = nullptr;
}

void Scene_Update(float delta_time)
{
	// After call Scane_Change at some point, and update will be called.
	// Then it will change scene automatically;
	// :: Pretty clever, if you ask me.
	if (m_pCurrentScene != m_pNextScene)
	{
		delete m_pCurrentScene;
		m_pCurrentScene = m_pNextScene;

		m_pCurrentScene->Initialize();
		//Scene_Initialize();
	}

	m_pCurrentScene->Update(delta_time);
}

void Scene_Draw()
{
	m_pCurrentScene->Draw();
}

void Scene_Change(Scene* pNextScene)
{
	m_pNextScene = pNextScene;
}
