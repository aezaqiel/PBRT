#pragma once

#include "Scene.hpp"

class Camera
{
public:
    Camera(usize width, usize height, f32 near, f32 far);
    ~Camera() = default;

    inline void SetScene(Scene* scene)
    {
        m_Scene = scene;
    }

    std::vector<glm::vec3> Render(usize samples, usize depth);

private:
    Ray GetRay(usize i, usize j);
    glm::vec3 RayColor(const Ray& ray, usize depth);

private:
    usize m_Width;
    usize m_Height;
    Interval m_Clip;

    glm::vec3 m_Center;

    glm::vec3 m_PixelDeltaU;
    glm::vec3 m_PixelDeltaV;
    glm::vec3 m_Pixel00Loc;

    Scene* m_Scene { nullptr };
};
