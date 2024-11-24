#pragma once

#include "interval.h"
#include "vec3.h"

#include <iostream>

typedef vec3 color;

inline double linear_to_gamma(double linear_component) {
    if (linear_component > 0) {
        return std::sqrt(linear_component);
    }
    return 0;
}

void write_color(std::ostream& out, color pixel_color); 