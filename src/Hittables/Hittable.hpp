#pragma once

#include "Containers/Ray.hpp"
#include "Containers/Interval.hpp"
#include "Containers/AABB.hpp"

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

class Hittable
{
public:
    virtual ~Hittable() = default;
    virtual std::optional<HitRecord> Hit(const Ray& ray, const Interval& clip) const = 0;
    virtual AABB BBox() const = 0;
};

template <typename T>
concept IsHittable = std::is_base_of_v<Hittable, T>;
