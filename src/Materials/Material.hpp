#pragma once

#include "Primitives/HitRecord.hpp"

class Material
{
public:
    virtual ~Material() = default;

    virtual bool Scatter(const Ray& ray, const HitRecord& record, glm::vec3& attenuation, Ray& scattered) const
    {
        return false;
    }
};
