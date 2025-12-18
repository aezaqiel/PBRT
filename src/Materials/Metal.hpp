#pragma once

#include "Material.hpp"

class Metal final : public Material
{
public:
    Metal(const glm::vec3& albedo, f32 fuzz);
    virtual ~Metal() = default;

    virtual std::optional<std::pair<glm::vec3, Ray>> Scatter(const Ray& ray, const HitRecord& hit) const override;

private:
    glm::vec3 m_Albedo;
    f32 m_Fuzz;
};
