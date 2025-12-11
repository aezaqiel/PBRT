#pragma once

#include "Primitives/Sphere.hpp"

class Scene
{
public:
    Scene() = default;

    template <typename T, typename... Args>
        requires std::is_same_v<T, Sphere> && std::is_constructible_v<T, Args...>
    void Push(Args&&... args)
    {
        if constexpr (std::is_same_v<T, Sphere>) {
            m_Spheres.emplace_back(std::forward<Args>(args)...);
        }
    }

    bool Hit(const Ray& ray, Interval clip, HitRecord& record) const;

private:
    std::vector<Sphere> m_Spheres;
};
