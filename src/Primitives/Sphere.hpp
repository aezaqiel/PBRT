#pragma once

#include "HitRecord.hpp"
#include "Containers/Interval.hpp"

class Sphere
{
public:
    Sphere(const glm::vec3& center, f32 radius, MaterialHandle material = MaterialHandle());
    ~Sphere() = default;

    bool Hit(const Ray& ray, Interval t, HitRecord& record) const;

private:
    glm::vec3 m_Center;
    f32 m_Radius;

    MaterialHandle m_Material;
};
