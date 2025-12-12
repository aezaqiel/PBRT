#include "Camera.hpp"

#include "Core/Rng.hpp"
#include "Materials/Material.hpp"

Camera::Camera(usize width, usize height, f32 vFov, const glm::vec3& lookfrom, const glm::vec3& lookat, f32 defocusAngle, f32 focusDistance, const Interval& clip)
    : m_Width(width), m_Height(height), m_Clip(clip), m_Center(lookfrom), m_DefocusAngle(defocusAngle)
{
	f32 aspectRatio = static_cast<f32>(m_Width) / static_cast<f32>(m_Height);

    glm::vec3 direction = lookfrom - lookat;

    f32 theta = glm::radians(vFov);
    f32 h = std::tan(theta / 2.0f);

	f32 viewportHeight = 2.0f * h * focusDistance;
	f32 viewportWidth = viewportHeight * (aspectRatio);

    glm::vec3 w = glm::normalize(direction);
    glm::vec3 u = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), w));
    glm::vec3 v = glm::cross(w, u);

	glm::vec3 viewportU = viewportWidth * u;
	glm::vec3 viewportV = viewportHeight * -v;

	m_PixelDeltaU = viewportU / static_cast<f32>(m_Width);
	m_PixelDeltaV = viewportV / static_cast<f32>(m_Height);

	glm::vec3 viewportUpperLeft = m_Center - w * focusDistance - viewportU / 2.0f - viewportV / 2.0f;
	m_Pixel00Loc = viewportUpperLeft + 0.5f * (m_PixelDeltaU + m_PixelDeltaV);

    f32 defocusRadius = focusDistance * std::tan(glm::radians(defocusAngle / 2.0f));
    m_DefocusU = u * defocusRadius;
    m_DefocusV = v * defocusRadius;
}

std::vector<glm::vec3> Camera::Render(usize samples, usize depth) const
{
    std::vector<glm::vec3> buffer(m_Width * m_Height, glm::vec3(0.0f));

    if (!m_Scene) {
        std::println(std::cerr, "Scene not set...");
        return buffer;
    }

    std::println("Rendering:");
    std::println(" - Resolution: {}, {}", m_Width, m_Height);
    std::println(" - Samples: {}", samples);
    std::println(" - Depth: {}", depth);

    auto start = std::chrono::steady_clock::now();

    for (usize j = 0; j < m_Height; ++j) {
        constexpr i32 barWidth = 50;
        f32 progress = static_cast<f32>(j) / static_cast<f32>(m_Height);
        i32 pos = static_cast<i32>(barWidth * progress);

        std::print(std::clog, "\r[");
        for (i32 b = 0; b < barWidth; ++b) {
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

    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<f32> duration = end - start;
    f32 seconds = duration.count();

    std::println("Render time: {:.2f} s", seconds);

    return buffer;
}

glm::vec3 Camera::DefocusDiskSample() const
{
    glm::vec2 p = Random::InUnitDisk();
    return m_Center + p[0] * m_DefocusU + p[1] * m_DefocusV;
}

Ray Camera::GetRay(usize i, usize j) const
{
    glm::vec3 offset(Random::Float32() - 0.5f, Random::Float32() - 0.5f, 0.0f);
    glm::vec3 pixelSample = m_Pixel00Loc + ((static_cast<f32>(i) + offset.x) * m_PixelDeltaU) + ((static_cast<f32>(j) + offset.y) * m_PixelDeltaV);

    glm::vec3 origin = (m_DefocusAngle <= 0.0f) ? m_Center : DefocusDiskSample();
    glm::vec3 direction = pixelSample - origin;

    return Ray(origin, direction);
}

glm::vec3 Camera::RayColor(Ray ray, usize depth) const
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
