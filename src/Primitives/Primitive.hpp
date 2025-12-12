#pragma once

#include "Containers/AABB.hpp"
#include "Sphere.hpp"

using PrimitiveVariant = std::variant<
    Sphere
>;

template <typename T>
concept Hittable = requires(const T& t, const Ray& r, Interval clip, HitRecord& rec)
{
    { t.Hit(r, clip, rec) } -> std::convertible_to<bool>;
    { t.BBox() } -> std::convertible_to<AABB>;
};

template <typename T>
concept Primitive = IsVariantMember<T, PrimitiveVariant>::value && Hittable<T>;
