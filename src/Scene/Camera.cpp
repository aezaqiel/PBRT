#include "Camera.hpp"

#include "Core/Rng.hpp"
#include "Materials/Material.hpp"

Camera::Camera(usize width, usize height, f32 vFov, const glm::vec3& lookfrom, const glm::vec3& lookat, const Interval& clip)
    : m_Width(width), m_Height(height), m_Clip(clip), m_Center(lookfrom)
{
	f32 aspectRatio = static_cast<f32>(m_Width) / static_cast<f32>(m_Height);

    glm::vec3 direction = lookfrom - lookat;

	f32 focalLength = glm::length(direction);
    f32 theta = glm::radians(vFov);
    f32 h = std::tan(theta / 2.0f);

	f32 viewportHeight = 2.0f * h * focalLength;
	f32 viewportWidth = viewportHeight * (aspectRatio);

    glm::vec3 w = glm::normalize(direction);
    glm::vec3 u = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), w));
    glm::vec3 v = glm::cross(w, u);

	glm::vec3 viewportU = viewportWidth * u;
	glm::vec3 viewportV = viewportHeight * -v;

	m_PixelDeltaU = viewportU / static_cast<f32>(m_Width);
	m_PixelDeltaV = viewportV / static_cast<f32>(m_Height);

	glm::vec3 viewportUpperLeft = m_Center - w * focalLength - viewportU / 2.0f - viewportV / 2.0f;
	m_Pixel00Loc = viewportUpperLeft + 0.5f * (m_PixelDeltaU + m_PixelDeltaV);
}

std::vector<glm::vec3> Camera::Render(usize samples, usize depth)
{
    std::println("Rendering:");
    std::println(" - Resolution: {}, {}", m_Width, m_Height);
    std::println(" - Samples: {}", samples);
    std::println(" - Depth: {}", depth);

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

glm::vec3 Camera::RayColor(Ray ray, usize depth)
{
    glm::vec3 accumulated(1.0f);
    glm::vec3 color(0.0f);

    for (usize i = 0; i < depth; ++i) {
        HitRecord record;

        if (m_Scene->Hit(ray, m_Clip, record)) {
            glm::vec3 attenuation;
            Ray scattered;

            bool scatter = std::visit([&](const auto& mat) -> bool {
                return mat.Scatter(ray, record, attenuation, scattered);
            }, m_Scene->GetMaterial(record.material));

            if (scatter) {
                accumulated *= attenuation;
                ray = scattered;
            } else {
                return glm::vec3(0.0f);
            }
        } else {
            f32 a = 0.5f * (glm::normalize(ray.direction).y + 1.0f);
            glm::vec3 sky = glm::mix(glm::vec3(1.0f), glm::vec3(0.5f, 0.7f, 1.0f), a);

            return accumulated * sky;
        }
    }

    return glm::vec3(0.0f);
}
