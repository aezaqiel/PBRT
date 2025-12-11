#pragma once

#include "Primitives/HitRecord.hpp"

enum class MaterialType : u8
{
    None,
    Lambertian
};

struct Material
{
    MaterialType type { MaterialType::None };
    glm::vec3 albedo { 0.0f };

    bool Scatter(const Ray& ray, const HitRecord& record, glm::vec3& attenuation, Ray& scattered) const;
};
