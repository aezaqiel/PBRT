#include <stb_image_write.h>

#include "Ray.hpp"

#include "Geometry/Hittable.hpp"
#include "Geometry/HittableList.hpp"
#include "Geometry/Sphere.hpp"

glm::vec3 RayColor(const Ray& ray, const Hittable& scene)
{
	HitRecord record;
	if (scene.Hit(ray, 0.0001f, std::numeric_limits<f32>::infinity(), record)) {
		return 0.5f * (record.normal + 1.0f);
	}

	glm::vec3 unitDirection = glm::normalize(ray.direction);
	f32 a = 0.5f * (unitDirection.y + 1.0f);

	return glm::mix(glm::vec3(1.0f), glm::vec3(0.5f, 0.7f, 1.0f), a);
}

int main()
{
    constexpr usize IMAGE_WIDTH = 1280;
    constexpr usize IMAGE_HEIGHT = 720;
    constexpr usize SAMPLES = 64;
    constexpr usize DEPTH = 8;

  	std::vector<u8> buffer(IMAGE_WIDTH * IMAGE_HEIGHT * 3);

	HittableList scene;
	scene.Push(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f));
	scene.Push(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f));

	f32 aspectRatio = static_cast<f32>(IMAGE_WIDTH) / static_cast<f32>(IMAGE_HEIGHT);

	f32 focalLength = 1.0f;
	f32 viewportHeight = 2.0f;
	f32 viewportWidth = viewportHeight * (aspectRatio);

	glm::vec3 cameraCenter(0.0f);

	glm::vec3 viewportU = glm::vec3(viewportWidth, 0.0f, 0.0f);
	glm::vec3 viewportV = glm::vec3(0.0f, -viewportHeight, 0.0f);

	glm::vec3 pixelDeltaU = viewportU / static_cast<f32>(IMAGE_WIDTH);
	glm::vec3 pixelDeltaV = viewportV / static_cast<f32>(IMAGE_HEIGHT);

	glm::vec3 viewportUpperLeft = cameraCenter - glm::vec3(0.0f, 0.0f, focalLength) - viewportU / 2.0f - viewportV / 2.0f;
	glm::vec3 pixel00Loc = viewportUpperLeft + 0.5f * (pixelDeltaU + pixelDeltaV);

  	for (usize j = 0; j < IMAGE_HEIGHT; ++j) {
		std::clog << "\rScanlines remaining: " << (IMAGE_HEIGHT - 1) << ' ' << std::flush;
    	for (usize i = 0; i < IMAGE_WIDTH; ++i) {
			glm::vec3 pixelCenter = pixel00Loc + (static_cast<f32>(i) * pixelDeltaU) + (static_cast<f32>(j) * pixelDeltaV);
			glm::vec3 rayDirection = pixelCenter - cameraCenter;

			glm::vec3 color = RayColor(Ray(cameraCenter, rayDirection), scene);

			usize index = (i + j * IMAGE_WIDTH) * 3;

			buffer[index + 0] = static_cast<u8>(color.r * 255.0f);
			buffer[index + 1] = static_cast<u8>(color.g * 255.0f);
			buffer[index + 2] = static_cast<u8>(color.b * 255.0f);
    	}
  	}
	std::clog << "\rDone                                                   " << std::endl;

  	if (stbi_write_png("image.png", IMAGE_WIDTH, IMAGE_HEIGHT, 3, buffer.data(), IMAGE_WIDTH * 3)) {
		std::println("Image saved to image.png");
	} else {
		std::println("Failed to save image");
	}
}
