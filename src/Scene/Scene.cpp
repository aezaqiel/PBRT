#include "Scene.hpp"

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
