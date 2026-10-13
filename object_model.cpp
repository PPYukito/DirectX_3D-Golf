#include "object_model.h"
#include "model.h"
#include "SimdMath.h"

void ModelObject::Draw() const
{
	ModelDraw(m_model, GetPosition(), GetRotation());
}
