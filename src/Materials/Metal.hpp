#pragma once

#include <glm/glm.hpp>

#include "Primitives/HitRecord.hpp"
#include "Containers/Ray.hpp"

class Metal
{
public:
    Metal(const glm::vec3& albedo);

    bool Scatter(const Ray& ray, const HitRecord& record, glm::vec3& attenuation, Ray& scattered) const;

private:
    glm::vec3 m_Albedo;
};
