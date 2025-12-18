#pragma once

#include "Hittable.hpp"
#include "Materials/Material.hpp"

class Sphere final : public Hittable
{
public:
    Sphere(const glm::vec3& center, f32 radius, const std::shared_ptr<Material>& material);
    Sphere(const glm::vec3& c0, const glm::vec3& c1, f32 radius, const std::shared_ptr<Material>& material);

    ~Sphere() = default;

    inline virtual AABB BBox() const override { return m_BBox; }

    virtual std::optional<HitRecord> Hit(const Ray& ray, const Interval& clip) const override;

private:
    Ray m_Center;
    f32 m_Radius;

    std::shared_ptr<Material> m_Material;
    AABB m_BBox;
};
