#pragma once

#include "Containers/Ray.hpp"

class Material;

struct HitRecord
{
    glm::vec3 p;
    glm::vec3 normal;
    f32 t;
    bool frontFace;

    std::shared_ptr<Material> material;

    void SetFace(const Ray& ray, const glm::vec3& outNormal)
    {
        frontFace = glm::dot(ray.direction, outNormal) < 0.0f;
        normal = frontFace ? outNormal : -outNormal;
    }
};
