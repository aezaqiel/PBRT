#include "Sphere.hpp"

Sphere::Sphere(const glm::vec3& center, f32 radius, usize material)
    : m_Center(center), m_Radius(std::fmax(0.0f, radius)), m_Material(material)
{
}

bool Sphere::Hit(const Ray& ray, Interval t, HitRecord& record) const
{
    glm::vec3 oc = ray.origin - m_Center;

    f32 a = glm::dot(ray.direction, ray.direction);
    f32 halfB = glm::dot(oc, ray.direction);
    f32 c = glm::dot(oc, oc) - m_Radius * m_Radius;

    f32 discriminant = halfB * halfB - a * c;

    if (discriminant < 0.0f) {
        return false;
    }

    f32 sqrtd = glm::sqrt(discriminant);

    f32 root = (-halfB - sqrtd) / a;

    if (!t.Surrounds(root)) {
        root = (-halfB + sqrtd) / a;
        if (!t.Surrounds(root)) {
            return false;
        }
    }

    record.t = root;
    record.p = ray.At(root);
    record.SetFace(ray, (record.p - m_Center) / m_Radius);
    record.material = m_Material;

    return true;
}
