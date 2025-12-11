#pragma once

#include "HitRecord.hpp"
#include "Containers/Interval.hpp"

class Sphere
{
public:
    Sphere(const glm::vec3& center, f32 radius, const std::shared_ptr<Material>& material);
    ~Sphere() = default;

    bool Hit(const Ray& ray, Interval t, HitRecord& record) const;

private:
    glm::vec3 m_Center;
    f32 m_Radius;

    std::shared_ptr<Material> m_Material;
};
