#include "BVH.hpp"

#include <glm/glm.hpp>

namespace {

    glm::vec3 GetCentroid(const AABB& box)
    {
        f32 cx = (box.x.min + box.x.max) * 0.5f;
        f32 cy = (box.y.min + box.y.max) * 0.5f;
        f32 cz = (box.z.min + box.z.max) * 0.5f;
        return glm::vec3(cx, cy, cz);
    }

}

BVH::BVH(const std::vector<std::shared_ptr<Hittable>>& primitives)
    : AccelerationStructure(primitives)
{
}

std::optional<HitRecord> BVH::Hit(const Ray& ray, const Interval& clip) const
{
    if (m_Nodes.empty()) return std::nullopt;

    HitRecord record;
    Interval t(clip);

    bool hitAnything = false;

    struct StackItem { u32 index; };
    std::array<StackItem, 64> stack;
    i32 stackPtr = 0;

    stack[stackPtr++] = { 0 };

    while (stackPtr > 0) {
        u32 nodeIdx = stack[--stackPtr].index;
        const BVHNode& node = m_Nodes[nodeIdx];

        if (!node.bbox.Hit(ray, t)) {
            continue;
        }

        if (node.primCount > 0) {
            for (u32 i = 0; i < node.primCount; ++i) {
                const auto& hittable = m_Primitives[node.leftFirst + i];
                auto hit = hittable->Hit(ray, t);
                if (hit) {
                    hitAnything = true;
                    t.max = hit->t;
                    record = *hit;
                }
            }
        } else {
            stack[stackPtr++] = { node.leftFirst + 1 };
            stack[stackPtr++] = { node.leftFirst };
        }
    }

    if (!hitAnything) {
        return std::nullopt;
    }

    return record;
}

void BVH::Build()
{
    m_Nodes.clear();
    if (m_Primitives.empty()) return;

    m_Nodes.reserve(m_Primitives.size() * 2);

    BVHNode& root = m_Nodes.emplace_back();
    root.leftFirst = 0;
    root.primCount = static_cast<u32>(m_Primitives.size());
    root.bbox = m_BBox;

    BuildRecursive(0, 0, static_cast<u32>(m_Primitives.size()));

    std::println("BVH built: {} nodes for {} primitves", m_Nodes.size(), m_Primitives.size());
}

void BVH::BuildRecursive(u32 nodeIdx, u32 start, u32 end)
{
    u32 count = end - start;

    if (count <= 4) {
        AABB leaf;
        for (u32 i = start; i < end; ++i) {
            leaf = AABB(leaf, m_Primitives[i]->BBox());
        }

        m_Nodes[nodeIdx].bbox = leaf;
        m_Nodes[nodeIdx].leftFirst = start;
        m_Nodes[nodeIdx].primCount = count;

        return;
    }

    AABB centroid;
    for (u32 i = start; i < end; ++i) {
        glm::vec3 c = GetCentroid(m_Primitives[i]->BBox());
        AABB cb(c, c);
        centroid = AABB(cb, centroid);
    }

    usize axis = 0;
    f32 maxLen = centroid.x.Size();
    if (centroid.y.Size() > maxLen) { maxLen = centroid.y.Size(); axis = 1; }
    if (centroid.z.Size() > maxLen) { axis = 2; }

    f32 mid = (centroid.AxisInterval(axis).min + centroid.AxisInterval(axis).max) * 0.5f;

    auto it = std::partition(m_Primitives.begin() + start, m_Primitives.begin() + end,
        [&](const auto& obj) {
            glm::vec3 c = GetCentroid(obj->BBox());
            return c[axis] < mid;
        }
    );

    u32 splitIdx = static_cast<u32>(std::distance(m_Primitives.begin(), it));

    if (splitIdx == start || splitIdx == end) {
        splitIdx = start + (count / 2);
        std::nth_element(m_Primitives.begin() + start, m_Primitives.begin() + splitIdx, m_Primitives.begin() + end,
            [&](const auto& a, const auto& b) {
                return GetCentroid(a->BBox())[axis] < GetCentroid(b->BBox())[axis];
            }
        );
    }

    u32 leftChildIdx = static_cast<u32>(m_Nodes.size());

    m_Nodes.emplace_back();
    m_Nodes.emplace_back();

    m_Nodes[nodeIdx].leftFirst = leftChildIdx;
    m_Nodes[nodeIdx].primCount = 0;

    BuildRecursive(leftChildIdx, start, splitIdx);
    BuildRecursive(leftChildIdx + 1, splitIdx, end);

    m_Nodes[nodeIdx].bbox = AABB(m_Nodes[leftChildIdx].bbox, m_Nodes[leftChildIdx + 1].bbox);
}
