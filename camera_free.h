#pragma once

#include "camera.h"

class FreeCamera : public Camera
{
private:
    Vector3 m_position{};
    Vector3 m_front{};

public:
    FreeCamera() = default;
    FreeCamera(const Vector3& position, const Vector3& look_to)
        : m_position(position)
        , m_front(look_to.Normalize()) {
    }

    void SetPosition(const Vector3& position) { m_position = position; }
    void SetLookAt(const Vector3& lookAt) { m_front = lookAt; }

    void MoveForward(float distance);
    void MoveBackward(float distance);
    void MoveLeft(float distance);
    void MoveRight(float distance);
    void MoveUp(float distance);
    void MoveDown(float distance);

    void RotateYaw(float angle); 
    void RotatePitch(float angle);

    Matrix GetViewMatrix() const override;
    Matrix GetProjectionMatrix() const override;

    const Vector3& GetFront() const { return m_front; }
};