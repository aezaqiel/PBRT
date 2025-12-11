#pragma once

#include "Materials/Material.hpp"
#include "Primitives/Primitive.hpp"

class Scene
{
public:
    Scene();

    inline usize AddMaterial(const Material& material)
    {
        m_Materials.push_back(material);
        return m_Materials.size() - 1;
    }

    inline const Material& GetMaterial(usize index) const
    {
        return m_Materials.at(index);
    }

    template <Primitive T, typename... Args>
        requires std::is_constructible_v<T, Args...>
    void Push(Args&&... args)
    {
        if constexpr (std::is_same_v<T, Sphere>) {
            m_Spheres.emplace_back(std::forward<Args>(args)...);
        }
    }

    bool Hit(const Ray& ray, Interval clip, HitRecord& record) const;

private:
    std::vector<Material> m_Materials;
    std::vector<Sphere> m_Spheres;
};
