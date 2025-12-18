#pragma once

#include "BVHNode.hpp"

#include "Hittables/Hittable.hpp"
#include "Materials/Material.hpp"

#include "Containers/AABB.hpp"

class Scene
{
public:
    Scene();

    template <IsMaterial T, typename... Args>
        requires std::constructible_from<T, Args...>
    inline MaterialHandle CreateMaterial(Args&&... args)
    {
        m_Materials.push_back(std::make_unique<T>(std::forward<Args>(args)...));
        return MaterialHandle { m_Materials.size() - 1 };
    }

    inline const Material& GetMaterial(MaterialHandle handle) const
    {
        return *m_Materials[handle.index];
    }

    template <IsHittable T, typename... Args>
        requires std::constructible_from<T, Args...>
    inline void Push(Args&&... args)
    {
        auto& obj = m_Hittables.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        m_BBox = AABB(m_BBox, obj->BBox());
    }

    inline AABB BBox() const { return m_BBox; }

    void Build();

    std::optional<HitRecord> Hit(const Ray& ray, Interval clip) const;

    static std::unique_ptr<Scene> TestScene();
    static std::unique_ptr<Scene> RandomSpheres();

private:
    void BuildRecursive(u32 nodeIdx, u32 start, u32 end);

private:
    std::vector<std::unique_ptr<Hittable>> m_Hittables;
    std::vector<std::unique_ptr<Material>> m_Materials;

    std::vector<BVHNode> m_Nodes;

    AABB m_BBox;
};
