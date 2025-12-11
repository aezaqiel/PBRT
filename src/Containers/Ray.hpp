#pragma once

#include <glm/glm.hpp>

struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;

    Ray()
        : origin(0.0f), direction(0.0f)
    {
    }

    Ray(const glm::vec3& origin, const glm::vec3& direction)
        : origin(origin), direction(direction)
    {
    }

    glm::vec3 At(f32 t) const
    {
        return origin + t * direction;
    }
};
