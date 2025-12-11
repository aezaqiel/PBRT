#include "Scene.hpp"

Scene::Scene()
{
    // Default material
    m_Materials.push_back(Lambertian(glm::vec3(1.0f, 0.0f, 1.0f)));
}

bool Scene::Hit(const Ray& ray, Interval clip, HitRecord& record) const
{
    HitRecord scratch;
    bool hit = false;

    for (const auto& sphere : m_Spheres) {
        if (sphere.Hit(ray, clip, scratch)) {
            hit = true;
            clip.max = scratch.t;
            record = scratch;
        }
    }

    return hit;
}
