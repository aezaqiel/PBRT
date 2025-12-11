#include "Sphere.hpp"

#include <glm/gtx/intersect.hpp>

Sphere::Sphere(const glm::vec3& center, f32 radius)
    : m_Center(center), m_Radius(radius)
{
}

bool Sphere::Hit(const Ray& ray, Interval t, HitRecord& record) const
{
    glm::vec3 intersection;
    glm::vec3 normal;

    if (!glm::intersectRaySphere(ray.origin, glm::normalize(ray.direction), m_Center, m_Radius, intersection, normal)) {
        return false;
    }

    f32 distance = glm::distance(ray.origin, intersection);
    if (!t.surrounds(distance)) {
        return false;
    }

    record.p = intersection;
    record.t = distance;
    record.SetFace(ray, normal);

    return true;
}
