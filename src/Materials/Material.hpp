#pragma once

#include "Hittables/Hittable.hpp"

struct ScatterRecord
{
    glm::vec3 attenuation;
    Ray scattered;
};

class Material
{
public:
    virtual ~Material() = default;
    virtual std::optional<ScatterRecord> Scatter(const Ray& ray, const HitRecord& hit) const = 0;
};

template <typename T>
concept IsMaterial = std::is_base_of_v<Material, T>;
