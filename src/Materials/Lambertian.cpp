#include "Lambertian.hpp"

#include "Core/Rng.hpp"

Lambertian::Lambertian(const glm::vec3& albedo)
    : m_Albedo(albedo)
{
}

bool Lambertian::Scatter(const Ray& ray, const HitRecord& record, glm::vec3& attenuation, Ray& scattered) const
{
    glm::vec3 direction = record.normal + Random::UnitVec3f();

    constexpr f32 epsilon = std::numeric_limits<f32>::epsilon();
    if (glm::abs(direction.x) < epsilon && glm::abs(direction.y) < epsilon && glm::abs(direction.z) < epsilon) {
        direction = record.normal;
    }

    scattered = Ray(record.p, direction, ray.time);
    attenuation = m_Albedo;

    return true;
}