#include <stb_image_write.h>

#include "PathConfig.inl"

#include "Core/Rng.hpp"

#include "Scene/Scene.hpp"
#include "Scene/Camera.hpp"

#include "Hittables/Sphere.hpp"
#include "Hittables/AccelerationStructure/BVH.hpp"

#include "Materials/Lambertian.hpp"
#include "Materials/Metal.hpp"
#include "Materials/Dielectric.hpp"

namespace {

	constexpr glm::vec3 ACES(glm::vec3 x)
	{
		constexpr f32 a = 2.51f;
		constexpr f32 b = 0.03f;
		constexpr f32 c = 2.43f;
		constexpr f32 d = 0.59f;
		constexpr f32 e = 0.14f;

		return glm::clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0f, 1.0f);
	}

    std::pair<std::vector<std::shared_ptr<Hittable>>, std::vector<std::shared_ptr<Material>>> TestScene()
	{
		std::vector<std::shared_ptr<Hittable>> hittables;
		std::vector<std::shared_ptr<Material>> materials;

		hittables.push_back(std::make_shared<Sphere>(
			glm::vec3(0.0f, -1000.0f, 0.0f), 1000.0f,
			materials.emplace_back(std::make_shared<Lambertian>(glm::vec3(0.5f, 0.5f, 0.5f)))
		));

		hittables.push_back(std::make_shared<Sphere>(
			glm::vec3(0.0f, 1.0f, 0.0f), 1.0f,
			materials.emplace_back(std::make_shared<Dielectric>(1.5f))
		));

		hittables.push_back(std::make_shared<Sphere>(
			glm::vec3(-4.0f, 1.0f, 0.0f), 1.0f,
			materials.emplace_back(std::make_shared<Lambertian>(glm::vec3(0.4f, 0.2f, 0.1f)))
		));

		hittables.push_back(std::make_shared<Sphere>(
			glm::vec3(4.0f, 1.0f, 0.0f), 1.0f,
			materials.emplace_back(std::make_shared<Metal>(glm::vec3(0.7f, 0.6f, 0.5f), 0.0))
		));

		return std::make_pair(hittables, materials);
	}

    std::pair<std::vector<std::shared_ptr<Hittable>>, std::vector<std::shared_ptr<Material>>> RandomSpheres()
	{
		std::vector<std::shared_ptr<Hittable>> hittables;
		std::vector<std::shared_ptr<Material>> materials;

		hittables.push_back(std::make_shared<Sphere>(
			glm::vec3(0.0f, -1000.0f, 0.0f), 1000.0f,
			materials.emplace_back(std::make_shared<Lambertian>(glm::vec3(0.5f, 0.5f, 0.5f)))
		));

		hittables.push_back(std::make_shared<Sphere>(
			glm::vec3(0.0f, 1.0f, 0.0f), 1.0f,
			materials.emplace_back(std::make_shared<Dielectric>(1.5f))
		));

		hittables.push_back(std::make_shared<Sphere>(
			glm::vec3(-4.0f, 1.0f, 0.0f), 1.0f,
			materials.emplace_back(std::make_shared<Lambertian>(glm::vec3(0.4f, 0.2f, 0.1f)))
		));

		hittables.push_back(std::make_shared<Sphere>(
			glm::vec3(4.0f, 1.0f, 0.0f), 1.0f,
			materials.emplace_back(std::make_shared<Metal>(glm::vec3(0.7f, 0.6f, 0.5f), 0.0))
		));

		for (i32 a = -10; a < 10; ++a) {
			for (i32 b = -10; b < 10; ++b) {
				f32 choose = Random::Float32();
				glm::vec3 center(a + 0.9f * Random::Float32(), 0.2f, b + 0.9f * Random::Float32());

				if (glm::length(center - glm::vec3(4.0f, 0.2f, 0.0f)) > 0.9f) {
					if (choose < 0.8f) {
						glm::vec3 albedo = Random::Vec3f() * Random::Vec3f();
						glm::vec3 center2 = center + glm::vec3(0, Random::Float32(0.0f, 0.5f), 0.0f);

						hittables.push_back(std::make_shared<Sphere>(
							center, center2, 0.2f,
							materials.emplace_back(std::make_shared<Lambertian>(albedo))
						));
					} else if (choose < 0.95f) {
						glm::vec3 albedo = Random::Vec3f(0.5f, 1.0f);
						f32 fuzz = Random::Float32(0.0f, 0.5f);

						hittables.push_back(std::make_shared<Sphere>(
							center, 0.2f,
							materials.emplace_back(std::make_shared<Metal>(albedo, fuzz))
						));
					} else {
						hittables.push_back(std::make_shared<Sphere>(
							center, 0.2f,
							materials.emplace_back(std::make_shared<Dielectric>(1.5f))
						));
					}
				}
			}
		}

		return std::make_pair(hittables, materials);
	}

}

int main()
{
    constexpr usize IMAGE_WIDTH = 400;
    constexpr usize IMAGE_HEIGHT = 300;
    constexpr usize SAMPLES = 32;
    constexpr usize DEPTH = 8;

	constexpr f32 INV_GAMMA = 1.0f / 2.2f;

	std::string outputFile = (std::filesystem::path(PathConfig::OutputDir) / "image.png").string();

	auto [hittables, materials] = TestScene();
	std::shared_ptr<BVH> as = std::make_shared<BVH>(hittables);
	as->Build();

	std::unique_ptr<Scene> scene = std::make_unique<Scene>();
	scene->AddHittable(as);

	std::unique_ptr<Camera> camera = std::make_unique<Camera>(
		IMAGE_WIDTH, IMAGE_HEIGHT,
		20.0f,
		glm::vec3(13.0f, 2.0f, 3.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		0.6f,
		10.0f,
		Interval(0.001f, std::numeric_limits<f32>::infinity())
	);

	camera->SetScene(scene.get());

	std::vector<glm::vec3> buffer = camera->Render(SAMPLES, DEPTH);

	std::vector<u8> image(buffer.size() * 3);

	for (usize i = 0; i < buffer.size(); ++i) {
		glm::vec3 color = ACES(buffer[i]);

		color.r = glm::pow(color.r, INV_GAMMA);
		color.g = glm::pow(color.g, INV_GAMMA);
		color.b = glm::pow(color.b, INV_GAMMA);

		usize index = i * 3;

		image[index + 0] = static_cast<u8>(std::clamp(color.r * 255.0f, 0.0f, 255.0f));
		image[index + 1] = static_cast<u8>(std::clamp(color.g * 255.0f, 0.0f, 255.0f));
		image[index + 2] = static_cast<u8>(std::clamp(color.b * 255.0f, 0.0f, 255.0f));
	}

  	if (stbi_write_png(outputFile.c_str(), IMAGE_WIDTH, IMAGE_HEIGHT, 3, image.data(), IMAGE_WIDTH * 3)) {
		std::println("Image saved to {}", outputFile);
	} else {
		std::println(std::cerr, "Failed to save image");
	}
}
