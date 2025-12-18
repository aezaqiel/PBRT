#pragma once

#include "AccelerationStructure.hpp"
#include "Containers/AABB.hpp"

struct BVHNode
{
    AABB bbox;
    u32 leftFirst;
    u32 primCount;
};

class BVH : public AccelerationStructure
{
public:
    BVH(const std::vector<std::shared_ptr<Hittable>>& primitives);
    virtual ~BVH() = default;

    inline virtual AABB BBox() const override { return m_BBox; }

    virtual std::optional<HitRecord> Hit(const Ray& ray, const Interval& clip) const override;
    virtual void Build() override;

private:
    void BuildRecursive(u32 nodeIdx, u32 start, u32 end);

private:
    std::vector<BVHNode> m_Nodes;
    AABB m_BBox;
};
