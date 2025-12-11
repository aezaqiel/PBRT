#include "Lambertian.hpp"

#include "Core/Rng.hpp"

Lambertian::Lambertian(const glm::vec3& albedo)
    : m_Albedo(albedo)
{
}

bool Lambertian::Scatter(const Ray& ray, const HitRecord& record, glm::vec3& attenuation, Ray& scattered) const
{
    glm::vec3 direction = record.normal + Random::UnitVector();

    scattered = Ray(record.p, direction);
    attenuation = m_Albedo;

    return true;
}
