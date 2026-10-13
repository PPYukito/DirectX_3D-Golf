#include "course.h"

#include "texture.h"
#include "object_cube.h"
#include "object_model.h"
#include "random.h"
#include "SimdMath.h"

Course::ModelData Course::GetTreeModel()
{
	int num = GetRandomInteger(0, (static_cast<int>(m_pTrees.size()) - 1));
	return m_pTrees[num];
}

Course::ModelData Course::GetRockModel()
{
	int num = GetRandomInteger(0, (static_cast<int>(m_pRocks.size()) - 1));
	return m_pRocks[num];
}

Course::ModelData Course::GetBushModel()
{
	int num = GetRandomInteger(0, (static_cast<int>(m_pBushes.size()) - 1));
	return m_pBushes[num];
}

Course::ModelData Course::GetFlowerModel()
{
	int num = GetRandomInteger(0, (static_cast<int>(m_pFlowers.size()) - 1));
	return m_pFlowers[num];
}

Course::ModelData Course::GetGrassModel()
{
	int num = GetRandomInteger(0, (static_cast<int>(m_pGrass.size()) - 1));
	return m_pGrass[num];
}

//Course::ModelData Course::GetGroundModel()
//{
//	int num = GetRandomInteger(0, (static_cast<int>(m_pGrounds.size()) - 1));
//	return m_pGrounds[num];
//}

Course::ModelData Course::GetMushroomModel()
{
	int num = GetRandomInteger(0, (static_cast<int>(m_pMushrooms.size()) - 1));
	return m_pMushrooms[num];
}

void Course::Initialize()
{
	m_texture_normal_id = Texture_Load(L"asset/texture/cube_embedded_texture.png");

	//m_model_tree1 = ModelLoad("asset/model/Tree_01.fbx", 0.005f);
	// Faces in tree1 model is not 3 faces, so it creates errors;

	m_model_tree2 = ModelLoad("asset/model/Tree_02.fbx", GetRandomFloat( 0.004f, 0.005f));
	m_model_tree3 = ModelLoad("asset/model/Tree_03.fbx", GetRandomFloat( 0.004f, 0.005f));
	m_model_tree4 = ModelLoad("asset/model/Tree_04.fbx", GetRandomFloat( 0.003f, 0.005f));
	m_model_tree5 = ModelLoad("asset/model/Tree_05.fbx", GetRandomFloat( 0.004f, 0.005f));
	m_pTrees.push_back(Course::ModelData{ m_model_tree2, {-0.5f, 0.0f, -0.5f}, { 0.5f,  1.9f,  0.5f } });
	m_pTrees.push_back(Course::ModelData{ m_model_tree3, {-0.5f, 0.0f, -0.5f}, { 0.5f,  2.2f,  0.5f } });
	m_pTrees.push_back(Course::ModelData{ m_model_tree4, {-0.5f, 0.0f, -0.5f}, { 0.5f,  2.8f, 0.5f } });
	m_pTrees.push_back(Course::ModelData{ m_model_tree5, {-0.5f, 0.0f, -0.5f}, { 0.5f,  1.65f,  0.5f } });

	m_model_rock1 = ModelLoad("asset/model/Rock_01.fbx", 1.0f);
	m_model_rock2 = ModelLoad("asset/model/Rock_02.fbx", 1.0f);
	m_model_rock3 = ModelLoad("asset/model/Rock_03.fbx", 1.0f);
	m_model_rock4 = ModelLoad("asset/model/Rock_04.fbx", 1.0f);
	//m_model_rock5 = ModelLoad("asset/model/Rock_05.fbx", 1.0f);
	m_pRocks.push_back(Course::ModelData{ m_model_rock1, {-0.5f, 0.0f, -0.5f}, { 0.24f,  0.24f,  0.24f } });
	m_pRocks.push_back(Course::ModelData{ m_model_rock2, {-0.5f, 0.0f, -0.5f}, { 0.05f,  0.05f,  0.05f } });
	m_pRocks.push_back(Course::ModelData{ m_model_rock3, {-0.5f, 0.0f, -0.5f}, { 0.08f,  0.08f,  0.08f } });
	m_pRocks.push_back(Course::ModelData{ m_model_rock4, {-0.5f, 0.0f, -0.5f}, { 0.5f,  1.1f,  0.5f } });
	//m_pRocks.push_back(Course::ModelData{ m_model_rock5, {-0.5f, -0.5f, -0.5f}, { 0.5f,  1.9f,  0.5f } });

	m_model_branch = ModelLoad("asset/model/Branch_01.fbx", 1.0f);

	m_model_stump = ModelLoad("asset/model/Stump_01.fbx", 0.01f);

	//m_model_flag = ModelLoad("asset/model/Golf_flag.fbx", 1.0f);

	m_model_bush1 = ModelLoad("asset/model/Bush_01.fbx", 0.01f);
	m_model_bush2 = ModelLoad("asset/model/Bush_02.fbx", 0.01f);
	m_model_bush3 = ModelLoad("asset/model/Bush_03.fbx", 0.01f);
	m_pBushes.push_back(Course::ModelData{ m_model_bush1, {-0.5f, 0.0f, -0.5f}, { 0.5f,  0.8f,  0.5f } });
	m_pBushes.push_back(Course::ModelData{ m_model_bush2, {-0.5f, 0.0f, -0.5f}, { 0.5f,  0.5f,  0.5f } });
	m_pBushes.push_back(Course::ModelData{ m_model_bush3, {-0.5f, 0.0f, -0.5f}, { 0.2f,  0.2f,  0.2f } });

	m_model_flower1 = ModelLoad("asset/model/Flowers_01.fbx", 0.01f);
	m_model_flower2 = ModelLoad("asset/model/Flowers_02.fbx", 0.01f);
	m_pFlowers.push_back(Course::ModelData{ m_model_flower1,{-0.5f, 0.0f, -0.5f}, { 0.5f,  0.33f,  0.5f } });
	m_pFlowers.push_back(Course::ModelData{ m_model_flower2,{-0.5f, 0.0f, -0.5f}, { 0.5f,  0.3f,  0.5f } });

	m_model_grass1 = ModelLoad("asset/model/Grass_01.fbx", 0.01f);
	m_model_grass2 = ModelLoad("asset/model/Grass_02.fbx", 0.01f);
	m_pGrass.push_back(Course::ModelData{ m_model_grass1,{-0.5f, 0.0f, -0.5f}, { 0.1f,  0.1f,  0.1f } });
	m_pGrass.push_back(Course::ModelData{ m_model_grass2,{-0.5f, 0.0f, -0.5f}, { 0.1f,  0.1f,  0.1f } });

	//m_model_ground1 = ModelLoad("asset/model/Ground_01.fbx", 0.1f);
	//m_model_ground2 = ModelLoad("asset/model/Ground_02.fbx", 1.0f);
	//m_model_ground3 = ModelLoad("asset/model/Ground_03.fbx", 1.0f);
	//m_pGrounds.push_back(Course::ModelData{ m_model_ground1, {-0.5f, -0.5f, -0.5f}, { 0.5f,  1.9f,  0.5f } });
	//m_pGrounds.push_back(Course::ModelData{ m_model_ground2, {-0.5f, -0.5f, -0.5f}, { 0.5f,  1.9f,  0.5f } });
	//m_pGrounds.push_back(Course::ModelData{ m_model_ground3, {-0.5f, -0.5f, -0.5f}, { 0.5f,  1.9f,  0.5f } });

	m_model_mushroom1 = ModelLoad("asset/model/Mushroom_01.fbx", 0.01f);
	m_model_mushroom2 = ModelLoad("asset/model/Mushroom_02.fbx", 0.01f);
	m_pMushrooms.push_back(Course::ModelData{ m_model_mushroom1,{-0.5f, 0.0f, -0.5f}, { 0.15f,  0.32f,  0.15f } });
	m_pMushrooms.push_back(Course::ModelData{ m_model_mushroom1,{-0.5f, 0.0f, -0.5f}, { 0.15f,  0.32f,  0.15f } });
}

void Course::Finalize()
{
	ModelRelease(m_model_mushroom2);
	ModelRelease(m_model_mushroom1);

	//ModelRelease(m_model_ground3);
	//ModelRelease(m_model_ground2);
	//ModelRelease(m_model_ground1);

	ModelRelease(m_model_grass2);
	ModelRelease(m_model_grass1);

	ModelRelease(m_model_flower2);
	ModelRelease(m_model_flower1);

	ModelRelease(m_model_bush3);
	ModelRelease(m_model_bush2);
	ModelRelease(m_model_bush1);

	//ModelRelease(m_model_flag);

	ModelRelease(m_model_stump);

	ModelRelease(m_model_branch);

	//ModelRelease(m_model_rock5);
	ModelRelease(m_model_rock4);
	ModelRelease(m_model_rock3);
	ModelRelease(m_model_rock2);
	ModelRelease(m_model_rock1);

	ModelRelease(m_model_tree5);
	ModelRelease(m_model_tree4);
	ModelRelease(m_model_tree3);
	ModelRelease(m_model_tree2);
	//ModelRelease(m_model_tree1);

	Texture_Release(m_texture_normal_id);
}

void Course::Create(Kind kind, const Vector3& position, const Vector3& rotation, bool flag)
{
	if (flag)
	{
		//m_objects.emplace_back(std::make_unique<ModelObject>(position, rotation, m_model_flag, Vector3{ -0.01f, -0.01f, -0.01f }, Vector3{ 0.01f, 0.01f, 0.01f }));
		return;
	}

	switch (kind)
	{
		case Kind::kNormal:
		{
			m_objects.emplace_back(std::make_unique<CubeObject>(position, rotation,	m_texture_normal_id));
			return;
		}

		case Kind::kTree:
		{
			ModelData modelData = GetTreeModel();
			m_objects.emplace_back(std::make_unique<ModelObject>(position, rotation, modelData.model, modelData.aabb_min, modelData.aabb_max));
			break;
		}

		case Kind::kRock:
		{
			ModelData modelData = GetRockModel();
			m_objects.emplace_back(std::make_unique<ModelObject>(position, rotation, modelData.model, modelData.aabb_min, modelData.aabb_max));
			break;
		}

		case Kind::kBranch:
		{
			m_objects.emplace_back(std::make_unique<ModelObject>(position, rotation, m_model_branch, Vector3{ -0.5f, -0.5f, -0.5f }, Vector3{ 0.5f, 0.08f, 0.5f }));
			break;
		}

		case Kind::kStump:
		{
			m_objects.emplace_back(std::make_unique<ModelObject>(position, rotation, m_model_stump, Vector3{ -0.5f, -0.5f, -0.5f }, Vector3{ 0.5f, 0.55f, 0.5f }));
			break;
		}

		case Kind::kBush:
		{
			ModelData modelData = GetBushModel();
			m_objects.emplace_back(std::make_unique<ModelObject>(position, rotation, modelData.model, modelData.aabb_min, modelData.aabb_max));
			break;
		}

		case Kind::kFlower:
		{
			ModelData modelData = GetFlowerModel();
			m_objects.emplace_back(std::make_unique<ModelObject>(position, rotation, modelData.model, modelData.aabb_min, modelData.aabb_max));
			break;
		}

		case Kind::kGrass:
		{
			ModelData modelData = GetGrassModel();
			m_objects.emplace_back(std::make_unique<ModelObject>(position, rotation, modelData.model, modelData.aabb_min, modelData.aabb_max));
			break;
		}

		/*case Kind::kGround:
		{
			ModelData modelData = GetGroundModel();
			m_objects.emplace_back(std::make_unique<ModelObject>(position, rotation, modelData.model, modelData.aabb_min, modelData.aabb_max));
			break;
		}*/

		case Kind::kMushroom:
		{
			ModelData modelData = GetMushroomModel();
			m_objects.emplace_back(std::make_unique<ModelObject>(position, rotation, modelData.model, modelData.aabb_min, modelData.aabb_max));
			break;
		}
	}
		
	//m_objects.emplace_back(std::make_unique<ModelObject>(position, rotation, model));
}

void Course::Draw() const
{
	for (const auto& object : m_objects) {
		object->Draw();
	}
}

bool Course::IsOverlap(const AABB& aabb) const
{
	for (const auto& object : m_objects) {
		if (object->GetAABB().IsOverlap(aabb)) {
			return true;
		}
	}

	return false;
}

void Course::HitCheck(Ball* ball)
{
	for (const auto& object : m_objects) {

		//ball->GetAABB().IsHit(object->GetAABB());
		AABB::Hit hit = ball->GetAABB().IsHit(object->GetAABB());

		// Just collider physics
		if (hit.IsHit)
		{
			ball->OnHit(hit);
		}

		// draw shadow position
		if (ball->GetShadowAABB().IsOverlap(object->GetAABB()))
		{
			ball->OnShadowOverlap(object->GetAABB());
		}
	}
}