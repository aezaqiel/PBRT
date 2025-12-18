#pragma once

#include "Material.hpp"

class Dielectric final : public Material
{
public:
    Dielectric(f32 ri);
    virtual ~Dielectric() = default;

    virtual std::optional<std::pair<glm::vec3, Ray>> Scatter(const Ray& ray, const HitRecord& hit) const override;

private:
    f32 m_RefractionIndex;
};
