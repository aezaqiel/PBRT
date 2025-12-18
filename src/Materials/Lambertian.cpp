#include "Lambertian.hpp"

#include "Core/Rng.hpp"

Lambertian::Lambertian(const glm::vec3& albedo)
    : m_Albedo(albedo)
{
}

std::optional<ScatterRecord> Lambertian::Scatter(const Ray& ray, const HitRecord& hit) const
{
    glm::vec3 direction = hit.normal + Random::UnitVec3f();

    constexpr f32 epsilon = std::numeric_limits<f32>::epsilon();
    if (glm::abs(direction.x) < epsilon && glm::abs(direction.y) < epsilon && glm::abs(direction.z) < epsilon) {
        direction = hit.normal;
    }

    return ScatterRecord {
        .attenuation = m_Albedo,
        .scattered = Ray(hit.p, direction, ray.time)
    };
}
