#pragma once

#include "camera.h"

#include "aabb.h"

class FixedCamera : public Camera
{
private:
    Vector3 m_position{};
    Vector3 m_target_position{};
    AABB m_aabb{};

public:
    FixedCamera() = default;
    FixedCamera(const Vector3& position, const Vector3& target_position, const AABB& aabb)
        : m_position(position)
        , m_target_position(target_position)
        , m_aabb(aabb)
    {
    }

    Matrix GetViewMatrix() const override;
    Matrix GetProjectionMatrix() const override;

    Vector3 GetCameraPosition() const override {
        return m_position;
    }

    bool IsOverlap(const AABB& target_aabb) const {
        return m_aabb.IsOverlap(target_aabb);
    }

    void SetTargetPoisition(const Vector3& target_position)
    {
        m_target_position = target_position;
    }
};
