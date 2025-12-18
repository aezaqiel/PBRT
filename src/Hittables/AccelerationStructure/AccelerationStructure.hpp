#pragma once

#include "Hittables/Hittable.hpp"

class AccelerationStructure : public Hittable
{
public:
    virtual ~AccelerationStructure() = default;
    virtual void Build() = 0;

protected:
    AccelerationStructure(const std::vector<std::shared_ptr<Hittable>>& primitives);

protected:
    std::vector<std::shared_ptr<Hittable>> m_Primitives;
};
