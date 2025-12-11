#include <stb_image_write.h>

int main()
{
    constexpr usize IMAGE_WIDTH = 1280;
    constexpr usize IMAGE_HEIGHT = 720;
    constexpr usize SAMPLES = 64;
    constexpr usize DEPTH = 8;

  	std::vector<u8> buffer(IMAGE_WIDTH * IMAGE_HEIGHT * 3);

  	for (usize j = 0; j < IMAGE_HEIGHT; ++j) {
		std::clog << "\rScanlines remaining: " << (IMAGE_HEIGHT - 1) << ' ' << std::flush;
    	for (usize i = 0; i < IMAGE_WIDTH; ++i) {
			f32 r = static_cast<f32>(i) / static_cast<f32>(IMAGE_WIDTH - 1);
			f32 g = static_cast<f32>(j) / static_cast<f32>(IMAGE_HEIGHT - 1);
			f32 b = 0.25f;

			usize index = (i + j * IMAGE_WIDTH) * 3;

			buffer[index + 0] = static_cast<u8>(r * 255.0f);
			buffer[index + 1] = static_cast<u8>(g * 255.0f);
			buffer[index + 2] = static_cast<u8>(b * 255.0f);
    	}
  	}
	std::clog << "\rDone                                                   " << std::endl;

  	if (stbi_write_png("image.png", IMAGE_WIDTH, IMAGE_HEIGHT, 3, buffer.data(), IMAGE_WIDTH * 3)) {
		std::println("Image saved to image.png");
	} else {
		std::println("Failed to save image");
	}
}
