#pragma once

#include "Primitives/HitRecord.hpp"

#include "Lambertian.hpp"
#include "Metal.hpp"

template <typename T>
concept Scatterable = requires(const T& t, const Ray& r, const HitRecord& rec, glm::vec3& att, Ray& scat)
{
    { t.Scatter(r, rec, att, scat) } -> std::convertible_to<bool>;
};

using MaterialVariant = std::variant<
    Lambertian,
    Metal
>;

template <typename T>
concept Material = IsVariantMember<T, MaterialVariant>::value && Scatterable<T>;
