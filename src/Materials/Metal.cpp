#include "Metal.hpp"

#include "Core/Rng.hpp"

Metal::Metal(const glm::vec3& albedo, f32 fuzz)
    : m_Albedo(albedo), m_Fuzz(fuzz < 1.0f ? fuzz : 1.0f)
{
}

bool Metal::Scatter(const Ray& ray, const HitRecord& record, glm::vec3& attenuation, Ray& scattered) const
{
    glm::vec3 reflected = glm::reflect(ray.direction, record.normal);
    reflected = glm::normalize(reflected) + (m_Fuzz * Random::UnitVec3f());

    scattered = Ray(record.p, reflected);
    attenuation = m_Albedo;

    return glm::dot(scattered.direction, record.normal) > 0;
}
