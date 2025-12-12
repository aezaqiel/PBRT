#include "Scene.hpp"

#include "Core/Rng.hpp"

namespace {

    AABB GetPrimBox(const PrimitiveVariant& prim)
    {
        return std::visit([](const auto& p) { return p.BBox(); }, prim);
    }

    glm::vec3 GetCentroid(const AABB& box)
    {
        f32 cx = (box.x.min + box.x.max) * 0.5f;
        f32 cy = (box.y.min + box.y.max) * 0.5f;
        f32 cz = (box.z.min + box.z.max) * 0.5f;
        return glm::vec3(cx, cy, cz);
    }

}

Scene::Scene()
{
    // Default material
    m_Materials.push_back(Lambertian(glm::vec3(1.0f, 0.0f, 1.0f)));
}

bool Scene::Hit(const Ray& ray, Interval clip, HitRecord& record) const
{
    if (m_Nodes.empty()) return false;
    bool hitAnything = false;

    struct StackItem { u32 index; };
    std::array<StackItem, 64> stack;
    i32 stackPtr = 0;

    stack[stackPtr++] = { 0 };

    while (stackPtr > 0) {
        u32 nodeIdx = stack[--stackPtr].index;
        const BVHNode& node = m_Nodes[nodeIdx];

        if (!node.bbox.Hit(ray, clip)) {
            continue;
        }

        if (node.primCount > 0) {
            for (u32 i = 0; i < node.primCount; ++i) {
                const auto& primitive = m_Primitives[node.leftFirst + i];
                HitRecord scratch;

                bool hit = std::visit([&](const auto& prim) -> bool {
                    return prim.Hit(ray, clip, scratch);
                }, primitive);

                if (hit) {
                    hitAnything = true;
                    clip.max = scratch.t;
                    record = scratch;
                }
            }
        } else {
            stack[stackPtr++] = { node.leftFirst + 1 };
            stack[stackPtr++] = { node.leftFirst };
        }
    }

    return hitAnything;
}

void Scene::Build()
{
    m_Nodes.clear();
    if (m_Primitives.empty()) return;

    m_Nodes.reserve(m_Primitives.size() * 2);

    BVHNode& root = m_Nodes.emplace_back();
    root.leftFirst = 0;
    root.primCount = static_cast<u32>(m_Primitives.size());
    root.bbox = m_BBox;

    BuildRecursive(0, 0, static_cast<u32>(m_Primitives.size()));

    std::println("BVH built: {} nodes for {} primitives", m_Nodes.size(), m_Primitives.size());
}

std::unique_ptr<Scene> Scene::TestScene()
{
    std::unique_ptr<Scene> scene = std::make_unique<Scene>();

    scene->Push<Sphere>(
        glm::vec3(0.0f, -100.5f, -1.0f), 100.0f,
        scene->CreateMaterial<Lambertian>(glm::vec3(0.5f, 0.5f, 0.5f))
    );

    scene->Push<Sphere>(
        glm::vec3(0.0f, 0.0f, -1.2f), 0.5f,
        scene->CreateMaterial<Lambertian>(glm::vec3(0.4f, 0.2f, 0.1f))
    );

    scene->Push<Sphere>(
        glm::vec3(-1.0f, 0.0f, -1.0f), 0.5f,
        scene->CreateMaterial<Dielectric>(1.5f)
    );

    scene->Push<Sphere>(
        glm::vec3( 1.0f, 0.0f, -1.0f), 0.5f,
        scene->CreateMaterial<Metal>(glm::vec3(0.7f, 0.6f, 0.5f), 0.0)
    );

    return std::move(scene);
}

std::unique_ptr<Scene> Scene::RandomSpheres()
{
    std::unique_ptr<Scene> scene = std::make_unique<Scene>();

    scene->Push<Sphere>(
        glm::vec3(0.0f, -1000.0f, 0.0f), 1000.0f,
        scene->CreateMaterial<Lambertian>(glm::vec3(0.5f, 0.5f, 0.5f))
    );

    scene->Push<Sphere>(
        glm::vec3(0.0f, 1.0f, 0.0f), 1.0f,
        scene->CreateMaterial<Dielectric>(1.5f)
    );

    scene->Push<Sphere>(
        glm::vec3(-4.0f, 1.0f, 0.0f), 1.0f,
        scene->CreateMaterial<Lambertian>(glm::vec3(0.4f, 0.2f, 0.1f))
    );

    scene->Push<Sphere>(
        glm::vec3(4.0f, 1.0f, 0.0f), 1.0f,
        scene->CreateMaterial<Metal>(glm::vec3(0.7f, 0.6f, 0.5f), 0.0)
    );

    for (i32 a = -10; a < 10; ++a) {
        for (i32 b = -10; b < 10; ++b) {
            f32 choose = Random::Float32();
            glm::vec3 center(a + 0.9f * Random::Float32(), 0.2f, b + 0.9f * Random::Float32());

            if (glm::length(center - glm::vec3(4.0f, 0.2f, 0.0f)) > 0.9f) {
                if (choose < 0.8f) {
                    glm::vec3 albedo = Random::Vec3f() * Random::Vec3f();
                    glm::vec3 center2 = center + glm::vec3(0, Random::Float32(0.0f, 0.5f), 0.0f);

                    scene->Push<Sphere>(
                        center, center2, 0.2f,
                        scene->CreateMaterial<Lambertian>(albedo)
                    );
                } else if (choose < 0.95f) {
                    glm::vec3 albedo = Random::Vec3f(0.5f, 1.0f);
                    f32 fuzz = Random::Float32(0.0f, 0.5f);

                    scene->Push<Sphere>(
                        center, 0.2f,
                        scene->CreateMaterial<Metal>(albedo, fuzz)
                    );
                } else {
                    scene->Push<Sphere>(
                        center, 0.2f,
                        scene->CreateMaterial<Dielectric>(1.5f)
                    );
                }
            }
        }
    }

    return std::move(scene);
}

void Scene::BuildRecursive(u32 nodeIdx, u32 start, u32 end)
{
    u32 count = end - start;

    if (count <= 4) {
        AABB leaf;
        for (u32 i = start; i < end; ++i) {
            leaf = AABB(leaf, GetPrimBox(m_Primitives[i]));
        }

        m_Nodes[nodeIdx].bbox = leaf;
        m_Nodes[nodeIdx].leftFirst = start;
        m_Nodes[nodeIdx].primCount = count;

        return;
    }

    AABB centroid;
    for (u32 i = start; i < end; ++i) {
        glm::vec3 c = GetCentroid(GetPrimBox(m_Primitives[i]));
        AABB cb(c, c);
        centroid = AABB(cb, centroid);
    }

    usize axis = 0;
    f32 maxLen = centroid.x.Size();
    if (centroid.y.Size() > maxLen) { maxLen = centroid.y.Size(); axis = 1; }
    if (centroid.z.Size() > maxLen) { axis = 2; }

    f32 mid = (centroid.AxisInterval(axis).min + centroid.AxisInterval(axis).max) * 0.5f;

    auto it = std::partition(m_Primitives.begin() + start, m_Primitives.begin() + end,
        [&](const PrimitiveVariant& p) {
            glm::vec3 c = GetCentroid(GetPrimBox(p));
            return c[axis] < mid;
        }
    );

    u32 splitIdx = static_cast<u32>(std::distance(m_Primitives.begin(), it));

    if (splitIdx == start || splitIdx == end) {
        splitIdx = start + (count / 2);
        std::nth_element(m_Primitives.begin() + start, m_Primitives.begin() + splitIdx, m_Primitives.begin() + end,
            [&](const PrimitiveVariant& a, const PrimitiveVariant& b) {
                return GetCentroid(GetPrimBox(a))[axis] < GetCentroid(GetPrimBox(b))[axis];
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
