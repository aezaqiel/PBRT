#pragma once

#include "Material.hpp"

class Lambertian : public Material
{
public:
    Lambertian(const glm::vec3& albedo);

    virtual bool Scatter(const Ray& ray, const HitRecord& record, glm::vec3& attenuation, Ray& scattered) const override;

private:
    glm::vec3 m_Albedo;
};
