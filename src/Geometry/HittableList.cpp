#include "HittableList.hpp"

bool HittableList::Hit(const Ray& ray, f32 tmin, f32 tmax, HitRecord& record) const
{
    HitRecord scratchRecord;
    bool hit = false;
    f32 closest = tmax;

    for (const auto& object : m_Hittables) {
        if (object->Hit(ray, tmin, closest, scratchRecord)) {
            hit = true;
            closest = scratchRecord.t;
            record = scratchRecord;
        }
    }

    return hit;
}
