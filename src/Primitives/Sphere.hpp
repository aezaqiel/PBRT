#pragma once

#include "HitRecord.hpp"

#include "Containers/AABB.hpp"
#include "Containers/Interval.hpp"

class Sphere
{
public:
    Sphere(const glm::vec3& center, f32 radius, MaterialHandle material = MaterialHandle());
    Sphere(const glm::vec3& c0, const glm::vec3& c1, f32 radius, MaterialHandle material = MaterialHandle());

    ~Sphere() = default;

    inline AABB BBox() const { return m_BBox; }

    bool Hit(const Ray& ray, Interval t, HitRecord& record) const;

private:
    Ray m_Center;
    f32 m_Radius;

    MaterialHandle m_Material;
    AABB m_BBox;
};
