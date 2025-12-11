#include "HittableList.hpp"

bool HittableList::Hit(const Ray& ray, Interval t, HitRecord& record) const
{
    HitRecord scratchRecord;
    bool hit = false;

    for (const auto& object : m_Hittables) {
        if (object->Hit(ray, t, scratchRecord)) {
            hit = true;
            t.max = scratchRecord.t;
            record = scratchRecord;
        }
    }

    return hit;
}
