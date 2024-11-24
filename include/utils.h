#pragma once

#include <limits>
#include <random>

// Constants
constexpr double inf = std::numeric_limits<double>::infinity();
constexpr double pi = 3.1415926535897932385;

// Utility Functions
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180;
}

inline double random_double() {
    static std::random_device r;
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator(r());
    return distribution(generator);
}

inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

