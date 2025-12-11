#pragma once

#include "HitRecord.hpp"
#include "Containers/Interval.hpp"

class Sphere
{
public:
    Sphere(const glm::vec3& center, f32 radius, usize material = 0);
    ~Sphere() = default;

    bool Hit(const Ray& ray, Interval t, HitRecord& record) const;

private:
    glm::vec3 m_Center;
    f32 m_Radius;

    usize m_Material { 0 };
};
