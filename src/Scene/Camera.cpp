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

    constexpr usize TILE_SIZE = 32;

    usize numTilesX = (m_Width + TILE_SIZE - 1) / TILE_SIZE;
    usize numTilexY = (m_Height + TILE_SIZE - 1) / TILE_SIZE;
    usize totalTiles = numTilesX * numTilexY;
    usize tilesProcessed = 0;

    std::println("Rendering:");
    std::println(" - Resolution: {}, {}", m_Width, m_Height);
    std::println(" - Tiles: {} ({}x{})", totalTiles, TILE_SIZE, TILE_SIZE);
    std::println(" - Samples: {}", samples);
    std::println(" - Depth: {}", depth);

    auto start = std::chrono::steady_clock::now();

    for (usize ty = 0; ty < numTilexY; ++ty) {
        for (usize tx = 0; tx < numTilesX; ++tx) {
            usize xStart = tx * TILE_SIZE;
            usize yStart = ty * TILE_SIZE;

            usize xEnd = std::min(xStart + TILE_SIZE, m_Width);
            usize yEnd = std::min(yStart + TILE_SIZE, m_Height);

            for (usize j = yStart; j < yEnd; ++j) {
                for (usize i = xStart; i < xEnd; ++i) {
                    glm::vec3 color(0.0f);

                    for (usize s = 0; s < samples; ++s) {
                        color += RayColor(GetRay(i, j), depth);
                    }

                    color /= static_cast<f32>(samples);
                    buffer[i + j * m_Width] = color;
                }
            }

            tilesProcessed++;
            constexpr i32 barWidth = 50;
            f32 progress = static_cast<f32>(tilesProcessed) / static_cast<f32>(totalTiles);
            i32 pos = static_cast<i32>(barWidth * progress);

            std::print(std::clog, "\r[");
            for (i32 b = 0; b < barWidth; ++b) {
                if (b < pos) std::print(std::clog, "=");
                else if (b == pos) std::print(std::clog, ">");
                else std::print(std::clog, " ");
            }
            std::print(std::clog, "] {} %", static_cast<i32>(progress * 100.0f));
            std::clog << std::flush;
        }
    }

    std::println(std::clog, "\r[==================================================] 100 %");

    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<f64> duration = end - start;
    f64 seconds = duration.count();

    std::string timeStr;
    if (seconds < 60.0) {
        timeStr = std::format("{:.2f} s", seconds);
    } else if (seconds < 3600.0) {
        u64 m = static_cast<u64>(seconds) / 60;
        f64 s = seconds - static_cast<f64>(m * 60);
        timeStr = std::format("{} m {:.2f} s", m, s);
    } else {
        u64 h = static_cast<u64>(seconds) / 3600;
        u64 m = (static_cast<u64>(seconds) % 3600) / 60;
        f64 s = seconds - static_cast<f64>(h * 3600 + m * 60);
        timeStr = std::format("{} h {} m {:.2f} s", h, m, s);
    }

    u64 totalPrimaryRays = static_cast<u64>(m_Width) * m_Height * samples;
    f64 raysPerSec = totalPrimaryRays / seconds;

    std::string throughputStr;
    if (raysPerSec >= 1.0e6) {
        throughputStr = std::format("{:.2f} M", raysPerSec / 1.0e6);
    } else if (raysPerSec >= 1.0e3) {
        throughputStr = std::format("{:.2f} k", raysPerSec / 1.0e3);
    } else {
        throughputStr = std::format("{:.2f} ", raysPerSec);
    }

    std::println("Performance Metrics:");
    std::println(" - Duration:   {}", timeStr);
    std::println(" - Throughput: {} primary rays/s", throughputStr);
    std::println(" - Total Rays: {}", totalPrimaryRays);

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
    f32 time = Random::Float32();

    return Ray(origin, direction, time);
}

glm::vec3 Camera::RayColor(Ray ray, usize depth) const
{
    glm::vec3 accumulated(1.0f);
    glm::vec3 color(0.0f);

    for (usize i = 0; i < depth; ++i) {
        HitRecord record;

        auto hit = m_Scene->Hit(ray, m_Clip);
        if (hit) {
            auto scatter = hit->material->Scatter(ray, *hit);
            if (scatter) {
                auto [attenuation, scattered] = *scatter;
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
