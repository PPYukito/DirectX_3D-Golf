#pragma once

#include "SimdMath.h"


class Camera
{
private:

public:
    Camera() = default;
    virtual ~Camera() = default;

    virtual Matrix GetViewMatrix() const = 0;
    virtual Matrix GetProjectionMatrix() const = 0;
    virtual Vector3 GetCameraPosition() const { return { 0.0f, 0.0f, 0.0f }; };
    virtual const Vector3& GetFront() const { return { 0.0f, 0.0f, 0.0f }; };
};
