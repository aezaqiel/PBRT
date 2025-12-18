#include "Dielectric.hpp"

#include "Core/Rng.hpp"

namespace {

    f32 Reflectance(f32 cosine, f32 ri)
    {
        f32 r0 = (1.0f - ri) / (1.0f + ri);
        r0 = r0 * r0;
        return r0 + (1.0f - r0) * std::pow(1.0f - cosine, 5.0f);
    }

}

Dielectric::Dielectric(f32 ri)
    : m_RefractionIndex(ri)
{
}

std::optional<ScatterRecord> Dielectric::Scatter(const Ray& ray, const HitRecord& hit) const
{
    f32 ri = hit.frontFace ? (1.0f / m_RefractionIndex) : m_RefractionIndex;

    glm::vec3 unitDir = glm::normalize(ray.direction);
    f32 cosTheta = std::fmin(glm::dot(-unitDir, hit.normal), 1.0f);
    f32 sinTheta = std::sqrtf(1.0f - cosTheta * cosTheta);

    bool cannotRefract = ri * sinTheta > 1.0f;
    glm::vec3 direction;

    if (cannotRefract || Reflectance(cosTheta, ri) > Random::Float32()) {
        direction = glm::reflect(unitDir, hit.normal);
    } else {
        direction = glm::refract(unitDir, hit.normal, ri);
    }

    return ScatterRecord {
        .attenuation = glm::vec3(1.0f),
        .scattered = Ray(hit.p, direction, ray.time)
    };
}
