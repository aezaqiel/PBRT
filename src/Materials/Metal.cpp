#include "Metal.hpp"

#include "Core/Rng.hpp"

Metal::Metal(const glm::vec3& albedo, f32 fuzz)
    : m_Albedo(albedo), m_Fuzz(fuzz < 1.0f ? fuzz : 1.0f)
{
}

std::optional<std::pair<glm::vec3, Ray>> Metal::Scatter(const Ray& ray, const HitRecord& hit) const
{
    glm::vec3 reflected = glm::reflect(ray.direction, hit.normal);
    reflected = glm::normalize(reflected) + (m_Fuzz * Random::UnitVec3f());

    Ray scattered(hit.p, reflected, ray.time);
    glm::vec3 attenuation(m_Albedo);

    if (glm::dot(scattered.direction, hit.normal) <= 0) {
        return std::nullopt;
    }

    return std::optional(std::make_pair(attenuation, scattered));
}
