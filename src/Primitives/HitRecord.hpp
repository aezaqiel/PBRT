#pragma once

#include "Containers/Ray.hpp"

struct MaterialHandle
{
    // 0 index should be the default material
    usize index { 0 };

    MaterialHandle(usize index = 0)
        : index(index)
    {
    }
};

struct HitRecord
{
    glm::vec3 p;
    glm::vec3 normal;
    f32 t;
    bool frontFace;

    MaterialHandle material;

    void SetFace(const Ray& ray, const glm::vec3& outNormal)
    {
        frontFace = glm::dot(ray.direction, outNormal) < 0.0f;
        normal = frontFace ? outNormal : -outNormal;
    }
};
