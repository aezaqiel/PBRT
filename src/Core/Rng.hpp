#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

class Random
{
public:
    inline static u32 Uint32()
    {
        return Next();
    }

    inline static u32 Uint32(u32 min, u32 max)
    {
        return min + (Next() % (max - min + 1));
    }

    inline static f32 Float32()
    {
        u32 r = Next();
        u32 bits = (0x3F800000 | (r >> 9)); 
        return std::bit_cast<f32>(bits) - 1.0f;
    }

    inline static f32 Float32(f32 min, f32 max)
    {
        return min + (max - min) * Float32();
    }

    inline static glm::vec3 Vec3f()
    {
        return glm::vec3(Float32(), Float32(), Float32());
    }

    inline static glm::vec3 Vecf(f32 min, f32 max)
    {
        return glm::vec3(Float32(min, max), Float32(min, max), Float32(min, max));
    }

    inline static glm::vec3 UnitVec3f()
    {
        f32 z = Float32() * 2.0f - 1.0f;
        f32 a = Float32() * 2.0f * glm::pi<f32>();
        f32 r = std::sqrt(1.0f - z * z);
        
        f32 x = r * std::cos(a);
        f32 y = r * std::sin(a);
        
        return glm::vec3(x, y, z);
    }

    inline static glm::vec2 InUnitDisk()
    {
        f32 a = Float32() * 2.0f * glm::pi<f32>();
        f32 r = std::sqrtf(Float32());
        f32 x = r * std::cos(a);
        f32 y = r * std::sin(a);

        return glm::vec2(x, y);
    }

    inline static glm::vec3 InUnitDisk3D()
    {
        glm::vec2 p = InUnitDisk();
        return glm::vec3(p.x, p.y, 0.0f);
    }

private:
    struct XoshiroState
    {
        u32 s[4];

        XoshiroState()
        {
            std::random_device rd;
            u64 seed = (static_cast<u64>(rd()) << 32) | rd(); 

            auto splitmix64 = [&seed]() -> u32 {
                u64 z = (seed += 0x9e3779b97f4a7c15);
                z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
                z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
                return static_cast<u32>((z ^ (z >> 31)));
            };

            s[0] = splitmix64();
            s[1] = splitmix64();
            s[2] = splitmix64();
            s[3] = splitmix64();
        }
    };

    inline static u32 Next()
    {
        u32* s = s_State.s;

        const u32 result = std::rotl(s[0] + s[3], 7) + s[0];
        const u32 t = s[1] << 9;

        s[2] ^= s[0];
        s[3] ^= s[1];
        s[1] ^= s[2];
        s[0] ^= s[3];

        s[2] ^= t;
        s[3] = std::rotl(s[3], 11);

        return result;
    }

    inline static thread_local XoshiroState s_State;
};
