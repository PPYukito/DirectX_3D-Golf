#pragma once

class Scene {
public:
	virtual ~Scene() = default; // Parent need to be destroyed;
	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Update(float delta_time) = 0;
	virtual void Draw() const = 0;
};

void Scene_Initialize();
void Scene_Finalize();
void Scene_Update(float delta_time);
void Scene_Draw();

void Scene_Change(Scene* pNextScene);