#include <stb_image_write.h>

#include "PathConfig.inl"

#include "Scene/Scene.hpp"
#include "Scene/Camera.hpp"

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

}

int main()
{
    constexpr usize IMAGE_WIDTH = 400;
    constexpr usize IMAGE_HEIGHT = 300;
    constexpr usize SAMPLES = 32;
    constexpr usize DEPTH = 8;

	constexpr f32 INV_GAMMA = 1.0f / 2.2f;

	std::string outputFile = (std::filesystem::path(PathConfig::OutputDir) / "image.png").string();

	std::unique_ptr<Scene> scene = std::make_unique<Scene>();

	auto ground = scene->CreateMaterial<Lambertian>(glm::vec3(0.8f, 0.8f, 0.0f));
	auto center = scene->CreateMaterial<Lambertian>(glm::vec3(0.1f, 0.2f, 0.5f));
	auto left = scene->CreateMaterial<Metal>(glm::vec3(0.8f, 0.8f, 0.8f), 0.3f);
	auto right = scene->CreateMaterial<Metal>(glm::vec3(0.8f, 0.6f, 0.2f), 1.0f);

	scene->Push<Sphere>(glm::vec3( 0.0f, -100.5f, -1.0f), 100.0f, ground);
	scene->Push<Sphere>(glm::vec3( 0.0f,    0.0f, -1.2f), 0.5f, center);
	scene->Push<Sphere>(glm::vec3(-1.0f,    0.0f, -1.0f), 0.5f, left);
	scene->Push<Sphere>(glm::vec3( 1.0f,    0.0f, -1.0f), 0.5f, right);

	std::unique_ptr<Camera> camera = std::make_unique<Camera>(
		IMAGE_WIDTH, IMAGE_HEIGHT,
		0.001f, std::numeric_limits<f32>::infinity()
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
