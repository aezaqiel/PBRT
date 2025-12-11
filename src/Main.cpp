#include <stb_image_write.h>

#include "Geometry/HittableList.hpp"
#include "Geometry/Sphere.hpp"

#include "Camera.hpp"

int main()
{
    constexpr usize IMAGE_WIDTH = 1280;
    constexpr usize IMAGE_HEIGHT = 720;
    constexpr usize SAMPLES = 64;
    constexpr usize DEPTH = 8;

	HittableList scene;
	scene.Push(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f));
	scene.Push(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f));

	Camera camera(IMAGE_WIDTH, IMAGE_HEIGHT, 0.0001f, std::numeric_limits<f32>::infinity());
	std::vector<f32> buffer = camera.Render(scene);

	// TODO: Tonemapping
	std::vector<u8> image(buffer.size());
	for (usize i = 0; i < buffer.size(); i += 3) {
		image[i + 0] = static_cast<u8>(buffer[i + 0] * 255.0f);
		image[i + 1] = static_cast<u8>(buffer[i + 1] * 255.0f);
		image[i + 2] = static_cast<u8>(buffer[i + 2] * 255.0f);
	}

  	if (stbi_write_png("image.png", IMAGE_WIDTH, IMAGE_HEIGHT, 3, image.data(), IMAGE_WIDTH * 3)) {
		std::println("Image saved to image.png");
	} else {
		std::println("Failed to save image");
	}
}
