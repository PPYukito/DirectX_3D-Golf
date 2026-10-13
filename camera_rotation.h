#pragma once

#pragma once

#include "camera.h"

class RotationCamera : public Camera
{
private:
    Vector3 m_targetPosition{};
    float m_height{};
    float m_length{};
    float m_angleSpeed{};
    float m_angle{};

public:
    RotationCamera() = default;
    RotationCamera(const Vector3& target_position, float height, float length, float angleSpeed)
        : m_targetPosition(target_position)
        , m_height(height)
        , m_length(length)
        , m_angleSpeed(angleSpeed)
    {
    }

    void Update(float delta_time);

    Matrix GetViewMatrix() const override;
    Matrix GetProjectionMatrix() const override;

    void SetTarget(const Vector3& targetPosition);
};