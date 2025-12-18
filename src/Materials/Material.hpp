#pragma once

#include "Hittables/Hittable.hpp"

class Material
{
public:
    virtual ~Material() = default;
    virtual std::optional<std::pair<glm::vec3, Ray>> Scatter(const Ray& ray, const HitRecord& hit) const = 0;
};

template <typename T>
concept IsMaterial = std::is_base_of_v<Material, T>;
