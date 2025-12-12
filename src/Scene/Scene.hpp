#pragma once

#include "Materials/Material.hpp"
#include "Primitives/Primitive.hpp"

class Scene
{
public:
    Scene();

    template <Material T, typename... Args>
        requires std::constructible_from<T, Args...>
    inline MaterialHandle CreateMaterial(Args&&... args)
    {
        m_Materials.emplace_back(T(std::forward<Args>(args)...));
        return MaterialHandle { m_Materials.size() - 1 };
    }

    inline const MaterialVariant& GetMaterial(MaterialHandle handle) const
    {
        return m_Materials[handle.index];
    }

    template <Primitive T, typename... Args>
        requires std::constructible_from<T, Args...>
    inline void Push(Args&&... args)
    {
        m_Primitives.emplace_back(T(std::forward<Args>(args)...));
    }

    bool Hit(const Ray& ray, Interval clip, HitRecord& record) const;

    static std::unique_ptr<Scene> TestScene();
    static std::unique_ptr<Scene> RandomSpheres();

private:
    std::vector<MaterialVariant> m_Materials;
    std::vector<PrimitiveVariant> m_Primitives;
};
