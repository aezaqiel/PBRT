#include "Scene.hpp"

std::optional<HitRecord> Scene::Hit(const Ray& ray, Interval clip) const
{
    HitRecord record;
    bool hitAnything = false;

    for (const auto& hittable : m_Hittables) {
        auto hit = hittable->Hit(ray, clip);
        if (hit) {
            hitAnything = true;
            clip.max = hit->t;
            record = *hit;
        }
    }

    if (!hitAnything) {
        return std::nullopt;
    }

    return record;
}
