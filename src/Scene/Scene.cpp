#include "Scene.hpp"

#include "Core/Rng.hpp"

Scene::Scene()
{
    // Default material
    m_Materials.push_back(Lambertian(glm::vec3(1.0f, 0.0f, 1.0f)));
}

bool Scene::Hit(const Ray& ray, Interval clip, HitRecord& record) const
{
    HitRecord scratch;
    bool hitAnything = false;

    for (const auto& primitive : m_Primitives) {
        bool hit = std::visit([&](const auto& prim) -> bool {
            return prim.Hit(ray, clip, scratch);
        }, primitive);

        if (hit) {
            hitAnything = true;
            clip.max = scratch.t;
            record = scratch;
        }
    }

    return hitAnything;
}

std::unique_ptr<Scene> Scene::TestScene()
{
    std::unique_ptr<Scene> scene = std::make_unique<Scene>();

    scene->Push<Sphere>(
        glm::vec3(0.0f, -100.5f, -1.0f), 100.0f,
        scene->CreateMaterial<Lambertian>(glm::vec3(0.5f, 0.5f, 0.5f))
    );

    scene->Push<Sphere>(
        glm::vec3(0.0f, 0.0f, -1.2f), 0.5f,
        scene->CreateMaterial<Lambertian>(glm::vec3(0.4f, 0.2f, 0.1f))
    );

    scene->Push<Sphere>(
        glm::vec3(-1.0f, 0.0f, -1.0f), 0.5f,
        scene->CreateMaterial<Dielectric>(1.5f)
    );

    scene->Push<Sphere>(
        glm::vec3( 1.0f, 0.0f, -1.0f), 0.5f,
        scene->CreateMaterial<Metal>(glm::vec3(0.7f, 0.6f, 0.5f), 0.0)
    );

    return std::move(scene);
}

std::unique_ptr<Scene> Scene::RandomSpheres()
{
    std::unique_ptr<Scene> scene = std::make_unique<Scene>();

    scene->Push<Sphere>(
        glm::vec3(0.0f, -1000.0f, 0.0f), 1000.0f,
        scene->CreateMaterial<Lambertian>(glm::vec3(0.5f, 0.5f, 0.5f))
    );

    scene->Push<Sphere>(
        glm::vec3(0.0f, 1.0f, 0.0f), 1.0f,
        scene->CreateMaterial<Dielectric>(1.5f)
    );

    scene->Push<Sphere>(
        glm::vec3(-4.0f, 1.0f, 0.0f), 1.0f,
        scene->CreateMaterial<Lambertian>(glm::vec3(0.4f, 0.2f, 0.1f))
    );

    scene->Push<Sphere>(
        glm::vec3(4.0f, 1.0f, 0.0f), 1.0f,
        scene->CreateMaterial<Metal>(glm::vec3(0.7f, 0.6f, 0.5f), 0.0)
    );

    for (i32 a = -11; a < 11; ++a) {
        for (i32 b = -11; b < 11; ++b) {
            f32 choose = Random::Float32();
            glm::vec3 center(a + 0.9f * Random::Float32(), 0.2f, b + 0.9f * Random::Float32());

            if (glm::length(center - glm::vec3(4.0f, 0.2f, 0.0f)) > 0.9f) {
                if (choose < 0.8f) {
                    glm::vec3 albedo = Random::Vec3f() * Random::Vec3f();

                    scene->Push<Sphere>(
                        center, 0.2f,
                        scene->CreateMaterial<Lambertian>(albedo)
                    );
                } else if (choose < 0.95f) {
                    glm::vec3 albedo = Random::Vec3f(0.5f, 1.0f);
                    f32 fuzz = Random::Float32(0.0f, 0.5f);

                    scene->Push<Sphere>(
                        center, 0.2f,
                        scene->CreateMaterial<Metal>(albedo, fuzz)
                    );
                } else {
                    scene->Push<Sphere>(
                        center, 0.2f,
                        scene->CreateMaterial<Dielectric>(1.5f)
                    );
                }
            }
        }
    }

    return std::move(scene);
}
