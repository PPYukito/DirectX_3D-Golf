#pragma once

#include "object.h"
#include "model.h"

#include "aabb.h"


class ModelObject : public Object
{
private:
	MODEL* m_model{ nullptr };
	AABB m_aabb;

public:
	ModelObject() = default;
	ModelObject(const Vector3& position, const Vector3& rotation, MODEL* model)
		: Object(position, rotation)
		, m_model(model)
		, m_aabb{ {-0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, 0.5f}, position, rotation } { // need to fix aabb range;
	}
	ModelObject(const Vector3& position, const Vector3& rotation, MODEL* model, const Vector3& aabbMin, const Vector3& aabbMax)
		: Object(position, rotation)
		, m_model(model)
		, m_aabb{ aabbMin, aabbMax, position, rotation }
	{
	}
	~ModelObject() = default;

	void Draw() const override;

	AABB GetAABB() const override{
		/*AABB aabb = m_model->Aabb;
		aabb.SetPosition(GetPosition());
		return aabb;*/
		return m_aabb;
	}
};