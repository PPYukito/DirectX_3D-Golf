#pragma once

#include "camera.h"

class ShotCamera : public Camera
{
private:
    Vector3 m_BallPosition{};
    Vector3 m_cupPosition{};
    Vector3 m_front{};

public:
    ShotCamera() = default;
    ShotCamera(const Vector3& ball_position, const Vector3& cup_position)
        : m_BallPosition(ball_position)
        , m_cupPosition(cup_position)
        , m_front((cup_position - ball_position).Normalize()) {
    }

    void RotateYaw(float angle);
    //void RotatePitch(float angle);

    Matrix GetViewMatrix() const override;
    Matrix GetProjectionMatrix() const override;

    Vector3 GetCameraPosition() const override {
        constexpr float DITSTNACE_TO_BALL{ 2.0f };
        return { m_BallPosition - m_front * DITSTNACE_TO_BALL };
    }

    const Vector3& GetFront() const override
    {
        return m_front;
    }

    void SetBallPoisition(const Vector3& position)
    {
        m_BallPosition = position;
        m_front = (m_cupPosition - m_BallPosition).Normalize();
    }
};