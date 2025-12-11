#pragma once

#include "Hittable.hpp"

class Sphere final : public Hittable
{
public:
    Sphere(const glm::vec3& center, f32 radius);
    virtual ~Sphere() = default;

    virtual bool Hit(const Ray& ray, f32 tmin, f32 tmax, HitRecord& record) const override;

private:
    glm::vec3 m_Center;
    f32 m_Radius;
};
