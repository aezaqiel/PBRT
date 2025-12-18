#pragma once

#include "Hittable.hpp"

class Sphere final : public Hittable
{
public:
    Sphere(const glm::vec3& center, f32 radius, MaterialHandle material = MaterialHandle());
    Sphere(const glm::vec3& c0, const glm::vec3& c1, f32 radius, MaterialHandle material = MaterialHandle());

    ~Sphere() = default;

    inline virtual AABB BBox() const override { return m_BBox; }

    virtual std::optional<HitRecord> Hit(const Ray& ray, const Interval& clip) const override;

private:
    Ray m_Center;
    f32 m_Radius;

    MaterialHandle m_Material;
    AABB m_BBox;
};
