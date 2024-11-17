#pragma once

#include"utils.h" // inf is defined here

class interval {
public:
    double min, max;

    interval(): min(+inf), max(-inf) {}
    
    interval(double min, double max) : min(min), max(max) {}
    
    double size() const {
        return max - min;
    }

    bool surrounds(double x) const {
        return min < x && x < max;
    }

    double clamp(double x) const {
        return std::max(min, std::min(max, x));
    }

    static const interval empty, universe;
};

const interval interval::empty = interval(+inf, -inf);
const interval interval::universe = interval(-inf, +inf);
