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

    inline f32 size() const
    {
        return max - min;
    }

    inline bool contains(f32 x) const
    {
        return min <= x && x <= max;
    }

    inline bool surrounds(f32 x) const
    {
        return min < x && x < max;
    }

    static const Interval empty;
    static const Interval universe;
};
