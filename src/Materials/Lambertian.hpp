#pragma once

#include "Material.hpp"

class Lambertian final : public Material
{
public:
    Lambertian(const glm::vec3& albedo);
    virtual ~Lambertian() = default;

    virtual std::optional<ScatterRecord> Scatter(const Ray& ray, const HitRecord& hit) const override;
    bool Scatter(const Ray& ray, const HitRecord& record, glm::vec3& attenuation, Ray& scattered) const;

private:
    glm::vec3 m_Albedo;
};
