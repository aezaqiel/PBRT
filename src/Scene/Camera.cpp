#include "Camera.hpp"

#include "Core/Rng.hpp"

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
    std::println(std::clog, "Rendering:");
    std::println(std::clog, " - Resolution: {}, {}", m_Width, m_Height);
    std::println(std::clog, " - Samples: {}", samples);
    std::println(std::clog, " - Depth: {}", depth);

    std::vector<glm::vec3> buffer(m_Width * m_Height, glm::vec3(0.0f));

    if (!m_Scene) {
        std::println(std::cerr, "Scene not set...");
        return buffer;
    }

    for (usize j = 0; j < m_Height; ++j) {

        constexpr int barWidth = 50;
        f32 progress = static_cast<f32>(j) / static_cast<f32>(m_Height);
        int pos = static_cast<int>(barWidth * progress);

        std::print(std::clog, "\r[");
        for (int b = 0; b < barWidth; ++b) {
            if (b < pos) std::print(std::clog, "=");
            else if (b == pos) std::print(std::clog, ">");
            else std::print(std::clog, " ");
        }
        std::print(std::clog, "] {} %", static_cast<int>(progress * 100.0f));
        std::clog << std::flush;

        for (usize i = 0; i < m_Width; ++i) {
            glm::vec3 color(0.0f);

            for (usize s = 0; s < samples; ++s) {
                Ray ray = GetRay(i, j);
                color += RayColor(ray, depth);
            }

            color /= static_cast<f32>(samples);
            buffer[i + j * m_Width] = color;
        }
    }
    std::println(std::clog, "\r[==================================================] 100 %\nDone.");

    return buffer;
}

Ray Camera::GetRay(usize i, usize j)
{
    glm::vec3 offset(Random::Float32() - 0.5f, Random::Float32() - 0.5f, 0.0f);
    glm::vec3 pixelSample = m_Pixel00Loc + ((static_cast<f32>(i) + offset.x) * m_PixelDeltaU) + ((static_cast<f32>(j) + offset.y) * m_PixelDeltaV);

    return Ray(m_Center, pixelSample - m_Center);
}

glm::vec3 Camera::RayColor(const Ray& ray, usize depth)
{
    HitRecord record;
    if (m_Scene->Hit(ray, m_Clip, record)) {
        return 0.5f * RayColor(Ray(record.p, record.normal), depth - 1);
    }

    // Skybox
	f32 a = 0.5f * (glm::normalize(ray.direction).y + 1.0f);
	return glm::mix(glm::vec3(1.0f), glm::vec3(0.5f, 0.7f, 1.0f), a);
}
