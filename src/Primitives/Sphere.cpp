#include "Sphere.hpp"

Sphere::Sphere(const glm::vec3& center, f32 radius, MaterialHandle material)
    : m_Center(center, glm::vec3(0.0f)), m_Radius(std::fmax(0.0f, radius)), m_Material(material)
{
    glm::vec3 rvec(radius);
    m_BBox = AABB(center - rvec, center + rvec);
}

Sphere::Sphere(const glm::vec3& c0, const glm::vec3& c1, f32 radius, MaterialHandle material)
    : m_Center(c0, c1 - c0), m_Radius(std::fmax(0.0f, radius)), m_Material(material)
{
    glm::vec3 rvec(radius);
    AABB box1(m_Center.At(0.0f) - rvec, m_Center.At(0.0f) + rvec);
    AABB box2(m_Center.At(1.0f) - rvec, m_Center.At(1.0f) + rvec);
    m_BBox = AABB(box1, box2);
}

bool Sphere::Hit(const Ray& ray, Interval t, HitRecord& record) const
{
    glm::vec3 center = m_Center.At(ray.time);

    glm::vec3 oc = ray.origin - center;

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
    record.SetFace(ray, (record.p - center) / m_Radius);
    record.material = m_Material;

    return true;
}
