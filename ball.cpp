#include "ball.h"

#include "shadow_circle.h"

Ball::Ball(const Vector3& position)
	: m_pModel(ModelLoad("Asset/Model/Ball.fbx", 1.0f, true))
	, m_position(position)
{
	// Just set the min of aabb of ball's shadow to as low as we should;
	m_shadow_aabb = m_pModel->Aabb;
	Vector3 min = m_shadow_aabb.GetMin();
	/*min.x = min.x - 0.1f;
	min.z = min.z - 0.1f;*/
	min.y = -50.0f;

	m_shadow_aabb.SetMin(min);

	m_shadow_position.y = -50.0f;
}

Ball::~Ball()
{
	ModelRelease(m_pModel);
}

void Ball::Update(float delta_time)
{
	/*if (!canFall)
		return;*/
		
	// d—Í‚Éˆø‚©‚ê‚Ä—Ž‚Á‚±‚¿‚é
	m_velocity += Vector3{ 0.0f, -9.8f, 0.0f } *delta_time;
	m_position += m_velocity * delta_time;

	m_shadow_position.y = -50.0f;
}

void Ball::Draw() const
{
	ModelDraw(m_pModel, m_position, m_rotation);
	CircleShadow_Draw(m_shadow_position);
}

AABB Ball::GetAABB() const
{
	AABB aabb = m_pModel->Aabb;
	aabb.SetPosition(m_position);

	return aabb;
}

void Ball::OnHit(const AABB::Hit& hit)
{
	m_position = m_position - (hit.Depth * hit.Normal);

	m_velocity = m_velocity - 2.0f * m_velocity.Dot(hit.Normal) * hit.Normal;

	m_velocity = (m_velocity * 0.7f);
}

void Ball::AddImpulse(const Vector3& impulse)
{
	m_velocity += impulse / MASS;
}

bool Ball::IsStopped() const
{
	return m_velocity.LengthSq() <= EPSILON;
}

AABB Ball::GetShadowAABB() const
{
	AABB aabb = m_shadow_aabb;
	aabb.SetPosition(m_position);

	return aabb;
}

void Ball::OnShadowOverlap(const AABB& aabb)
{
	m_shadow_position.x = m_position.x;
	m_shadow_position.z = m_position.z;

	if (m_shadow_position.y < aabb.GetMax().y + aabb.GetPosition().y)
	{
		m_shadow_position.y = aabb.GetMax().y + aabb.GetPosition().y + 0.01f;
	}
}
