#include "Metal.hpp"

Metal::Metal(const glm::vec3& albedo)
    : m_Albedo(albedo)
{
}

bool Metal::Scatter(const Ray& ray, const HitRecord& record, glm::vec3& attenuation, Ray& scattered) const
{
    glm::vec3 reflected = glm::reflect(ray.direction, record.normal);

    scattered = Ray(record.p, reflected);
    attenuation = m_Albedo;

    return true;
}
