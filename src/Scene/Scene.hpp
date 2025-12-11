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
        m_Materials.emplace_back(std::forward<Args>(args)...);
        return MaterialHandle { m_Materials.size() - 1 };
    }

    inline const MaterialVariant& GetMaterial(MaterialHandle handle) const
    {
        return m_Materials.at(handle.index);
    }

    template <Primitive T, typename... Args>
        requires std::is_constructible_v<T, Args...>
    inline void Push(Args&&... args)
    {
        if constexpr (std::is_same_v<T, Sphere>) {
            m_Spheres.emplace_back(std::forward<Args>(args)...);
        }
    }

    bool Hit(const Ray& ray, Interval clip, HitRecord& record) const;

private:
    std::vector<MaterialVariant> m_Materials;
    std::vector<Sphere> m_Spheres;
};
