#include "Material.hpp"

#include "Core/Rng.hpp"

bool Material::Scatter(const Ray& ray, const HitRecord& record, glm::vec3& attenuation, Ray& scattered) const
{
    switch (type) {
        case MaterialType::Lambertian: {
            glm::vec3 direction = record.normal + Random::UnitVector();

            scattered = Ray(record.p, direction);
            attenuation = albedo;

            return true;
        } break;
        default:
            return false;
    }
}
