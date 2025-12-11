#pragma once

#include "Scene.hpp"

class Camera
{
public:
    Camera(usize width, usize height, f32 vFov, const glm::vec3& lookfrom, const glm::vec3& lookat, f32 defocusAngle, f32 focusDistance, const Interval& clip);
    ~Camera() = default;

    inline void SetScene(Scene* scene)
    {
        m_Scene = scene;
    }

    std::vector<glm::vec3> Render(usize samples, usize depth) const;

private:
    glm::vec3 DefocusDiskSample() const;
    Ray GetRay(usize i, usize j) const;

    glm::vec3 RayColor(Ray ray, usize depth) const;

private:
    usize m_Width;
    usize m_Height;
    Interval m_Clip;

    glm::vec3 m_Center;

    glm::vec3 m_PixelDeltaU;
    glm::vec3 m_PixelDeltaV;
    glm::vec3 m_Pixel00Loc;

    f32 m_DefocusAngle;
    glm::vec3 m_DefocusU;
    glm::vec3 m_DefocusV;

    Scene* m_Scene { nullptr };
};
