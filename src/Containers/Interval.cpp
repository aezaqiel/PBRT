#include "Interval.hpp"

const Interval Interval::empty = Interval(std::numeric_limits<f32>::infinity(), -std::numeric_limits<f32>::infinity());
const Interval Interval::universe = Interval(-std::numeric_limits<f32>::infinity(), std::numeric_limits<f32>::infinity());
