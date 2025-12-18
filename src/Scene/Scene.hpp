#pragma once

#include "Hittables/Hittable.hpp"
#include "Containers/AABB.hpp"

class Scene
{
public:
    Scene() = default;
    ~Scene() = default;

    inline void AddHittable(const std::shared_ptr<Hittable>& hittable)
    {
        m_Hittables.push_back(hittable);
        m_BBox = AABB(m_BBox, hittable->BBox());
    }

    inline AABB BBox() const { return m_BBox; }

    std::optional<HitRecord> Hit(const Ray& ray, Interval clip) const;

private:
    std::vector<std::shared_ptr<Hittable>> m_Hittables;

    AABB m_BBox;
};
