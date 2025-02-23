#pragma once

#include "interval.h"
#include "vec3.h"

#include <iostream>

typedef vec3 color; // r=x, g=y, b=z

inline color int_to_color(int r, int g, int b) {
    return color(r / 255.0, g / 255.0, b / 255.0);
}

inline double linear_to_gamma(double linear_component) {
    if (linear_component > 0) {
        return std::sqrt(linear_component);
    }
    return 0;
}

int get_color_component(color pixel_color, int component);

void write_color(std::ostream& out, color pixel_color); 