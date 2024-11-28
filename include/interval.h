#pragma once

#include "utils.h" // inf is defined here

class interval {
public:
    double min, max;

    interval(): min(+inf), max(-inf) {}
    interval(double min, double max) : min(min), max(max) {}
    interval(const interval& a, const interval& b) : min(std::min(a.min, b.min)), max(std::max(a.max, b.max)) {}

    
    inline double size() const {
        return max - min;
    }

    inline bool surrounds(double x) const {
        return min < x && x < max;
    }

    inline double clamp(double x) const {
        return std::max(min, std::min(max, x));
    }

    inline interval expand(double delta) const {
        auto padding = delta / 2;
        return {min - padding, max + padding};
    }

    static inline interval empty() {return {+inf, -inf};};
    static inline interval universe() {return {-inf, +inf};};
};

inline interval operator+(const interval& ival, double displacement) {
    return {ival.min + displacement, ival.max + displacement};
}

inline interval operator+(double displacement, const interval& ival) {
    return ival + displacement;
}