#pragma once

#include <glm/glm.hpp>

#include "Primitives/HitRecord.hpp"
#include "Containers/Ray.hpp"

class Dielectric
{
public:
    Dielectric(f32 ri);

    bool Scatter(const Ray& ray, const HitRecord& record, glm::vec3& attenuation, Ray& scattered) const;

private:
    f32 m_RefractionIndex;
};
