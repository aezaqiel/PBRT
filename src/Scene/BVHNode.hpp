#pragma once

#include "Containers/AABB.hpp"

struct BVHNode
{
    AABB bbox;
    u32 leftFirst;
    u32 primCount;
};
