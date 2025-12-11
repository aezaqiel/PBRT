#pragma once

struct Interval
{
    f32 min;
    f32 max;

    Interval()
        : min(std::numeric_limits<f32>::infinity()), max(-std::numeric_limits<f32>::infinity())
    {
    }

    Interval(f32 min, f32 max)
        : min(min), max(max)
    {
    }

    inline f32 Size() const
    {
        return max - min;
    }

    inline bool Contains(f32 x) const
    {
        return min <= x && x <= max;
    }

    inline bool Surrounds(f32 x) const
    {
        return min < x && x < max;
    }

    inline f32 Clamp(f32 x) const
    {
        return std::clamp(x, min, max);
    }

    static const Interval empty;
    static const Interval universe;
};
