#pragma once

#include "Sphere.hpp"

using PrimitiveVariant = std::variant<
    Sphere
>;

template <typename T, typename Variant>
struct IsVariantMember;

template <typename T, typename... Ts>
struct IsVariantMember<T, std::variant<Ts...>>
    : std::disjunction<std::is_same<T, Ts>...>
{
};

template <typename T>
concept Primitive = IsVariantMember<T, PrimitiveVariant>::value;
