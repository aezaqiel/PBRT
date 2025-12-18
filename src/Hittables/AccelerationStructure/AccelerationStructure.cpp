#include "AccelerationStructure.hpp"

AccelerationStructure::AccelerationStructure(const std::vector<std::shared_ptr<Hittable>>& primitives)
    : m_Primitives(primitives)
{
}
