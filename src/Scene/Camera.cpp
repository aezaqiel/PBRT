#include "Camera.hpp"


Camera::Camera(usize width, usize height, f32 near, f32 far)
    : m_Width(width), m_Height(height), m_Clip(near, far)
{
	f32 aspectRatio = static_cast<f32>(m_Width) / static_cast<f32>(m_Height);

	f32 focalLength = 1.0f;
	f32 viewportHeight = 2.0f;
	f32 viewportWidth = viewportHeight * (aspectRatio);

    m_Center = glm::vec3(0.0f);

	glm::vec3 viewportU = glm::vec3(viewportWidth, 0.0f, 0.0f);
	glm::vec3 viewportV = glm::vec3(0.0f, -viewportHeight, 0.0f);

	m_PixelDeltaU = viewportU / static_cast<f32>(m_Width);
	m_PixelDeltaV = viewportV / static_cast<f32>(m_Height);

	glm::vec3 viewportUpperLeft = m_Center - glm::vec3(0.0f, 0.0f, focalLength) - viewportU / 2.0f - viewportV / 2.0f;
	m_Pixel00Loc = viewportUpperLeft + 0.5f * (m_PixelDeltaU + m_PixelDeltaV);
}

std::vector<glm::vec3> Camera::Render(usize samples, usize depth)
{
    std::vector<glm::vec3> buffer(m_Width * m_Height, glm::vec3(0.0f));

    if (!m_Scene) {
        std::println("Scene not set...");
        return buffer;
    }

    for (usize j = 0; j < m_Height; ++j) {
		std::clog << "\rScanlines remaining: " << (m_Height - 1) << ' ' << std::flush;
        for (usize i = 0; i < m_Width; ++i) {
            glm::vec3 pixel = m_Pixel00Loc + (static_cast<f32>(i) * m_PixelDeltaU) + (static_cast<f32>(j) * m_PixelDeltaV);

            glm::vec3 color = RayColor(Ray(m_Center, pixel - m_Center));
            buffer[i + j * m_Width] = color;
        }
    }
	std::clog << "\rDone                                             " << std::endl;

    return buffer;
}

glm::vec3 Camera::RayColor(const Ray& ray)
{
    HitRecord record;
    if (m_Scene->Hit(ray, m_Clip, record)) {
        return 0.5f * (record.normal + 1.0f);
    }

    // Skybox
	f32 a = 0.5f * (glm::normalize(ray.direction).y + 1.0f);
	return glm::mix(glm::vec3(1.0f), glm::vec3(0.5f, 0.7f, 1.0f), a);
}
