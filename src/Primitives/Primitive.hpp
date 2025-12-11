#pragma once

#include "Sphere.hpp"

using PrimitiveVariant = std::variant<
    Sphere
>;

template <typename T>
concept Primitive = IsVariantMember<T, PrimitiveVariant>::value;
