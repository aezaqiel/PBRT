#pragma once

#include "Ray.hpp"

struct HitRecord
{
    glm::vec3 p;
    glm::vec3 normal;
    f32 t;
    bool frontFace;

    void SetFace(const Ray& ray, const glm::vec3& outNormal)
    {
        frontFace = glm::dot(ray.direction, outNormal) < 0.0f;
        normal = frontFace ? outNormal : -outNormal;
    }
};

class Hittable
{
public:
    virtual ~Hittable() = default;
    virtual bool Hit(const Ray& ray, f32 tmin, f32 tmax, HitRecord& record) const = 0;
};
