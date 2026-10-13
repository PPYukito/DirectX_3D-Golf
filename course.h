#pragma once

#include <vector>
#include <memory>
#include "object.h"
#include "model.h"
#include "ball.h"


class Course
{
public:
	enum class Kind
	{
		kNormal,
		// kSnow,
		// kWater,
		// kSand,
		kTree,
		kRock,
		kBranch,
		kBush,
		kFlower,
		kGrass,
		//kGround,
		kMushroom,
		kStump,
		kMax
	};

private:
	struct ModelData
	{
		MODEL* model;
		Vector3 aabb_min;
		Vector3 aabb_max;
	};

	std::vector<std::unique_ptr<Object>> m_objects;

	std::vector<ModelData> m_pTrees;
	std::vector<ModelData> m_pRocks;
	std::vector<ModelData> m_pBushes;
	std::vector<ModelData> m_pFlowers;
	std::vector<ModelData> m_pGrass;
	//std::vector<ModelData> m_pGrounds;
	std::vector<ModelData> m_pMushrooms;

	int m_texture_normal_id = -1;

	//MODEL* m_model_tree1 = nullptr;
	MODEL* m_model_tree2 = nullptr;
	MODEL* m_model_tree3 = nullptr;
	MODEL* m_model_tree4 = nullptr;
	MODEL* m_model_tree5 = nullptr;

	MODEL* m_model_rock1 = nullptr;
	MODEL* m_model_rock2 = nullptr;
	MODEL* m_model_rock3 = nullptr;
	MODEL* m_model_rock4 = nullptr;
	//MODEL* m_model_rock5 = nullptr;

	MODEL* m_model_branch = nullptr;

	MODEL* m_model_stump = nullptr;

	MODEL* m_model_flag = nullptr;

	MODEL* m_model_bush1 = nullptr;
	MODEL* m_model_bush2 = nullptr;
	MODEL* m_model_bush3 = nullptr;

	MODEL* m_model_flower1 = nullptr;
	MODEL* m_model_flower2 = nullptr;

	MODEL* m_model_grass1 = nullptr;
	MODEL* m_model_grass2 = nullptr;

	//MODEL* m_model_ground1 = nullptr;
	//MODEL* m_model_ground2 = nullptr;
	//MODEL* m_model_ground3 = nullptr;

	MODEL* m_model_mushroom1 = nullptr;
	MODEL* m_model_mushroom2 = nullptr;
	//Ball& ball;

private:
	ModelData GetTreeModel();
	ModelData GetRockModel();
	ModelData GetBushModel();
	ModelData GetFlowerModel();
	ModelData GetGrassModel();
	//ModelData GetGroundModel();
	ModelData GetMushroomModel();

public:
	void Initialize();
	void Finalize();
	void Create(Kind kind, const Vector3& position, const Vector3& rotation, bool flag = false);
	void Draw() const;

	bool IsOverlap(const AABB& aabb) const;

	void HitCheck(Ball* ball);
};
