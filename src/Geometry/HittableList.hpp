#pragma once

#include "Hittable.hpp"

class HittableList : public Hittable
{
public:
    HittableList() = default;
    virtual ~HittableList() = default;

    inline void Push(const std::shared_ptr<Hittable>& hittable)
    {
        m_Hittables.push_back(hittable);
    }

    inline void Reset()
    {
        m_Hittables.clear();
    }

    virtual bool Hit(const Ray& ray, Interval t, HitRecord& record) const override;

private:
    std::vector<std::shared_ptr<Hittable>> m_Hittables;
};
