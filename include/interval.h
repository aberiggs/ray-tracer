#pragma once

#include"utils.h" // inf is defined here

class interval {
public:
    double min, max;

    interval(): min(+inf), max(-inf) {}
    
    interval(double min, double max) : min(min), max(max) {}
    
    inline double size() const {
        return max - min;
    }

    inline bool surrounds(double x) const {
        return min < x && x < max;
    }

    inline double clamp(double x) const {
        return std::max(min, std::min(max, x));
    }

    static inline interval empty() {return {+inf, -inf};};
    static inline interval universe() {return {-inf, +inf};};
};
