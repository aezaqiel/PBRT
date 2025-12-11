#pragma once

#include "Scene.hpp"

class Camera
{
public:
    Camera(usize width, usize height, f32 near, f32 far);
    ~Camera() = default;

    std::vector<glm::vec3> Render(const Scene& scene);

private:
    glm::vec3 RayColor(const Ray& ray, const Scene& scene);

private:
    usize m_Width;
    usize m_Height;
    Interval m_Clip;

    glm::vec3 m_Center;

    glm::vec3 m_PixelDeltaU;
    glm::vec3 m_PixelDeltaV;
    glm::vec3 m_Pixel00Loc;
};
