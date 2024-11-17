#pragma once

#include "interval.h"
#include "vec3.h"

#include <iostream>

using color = vec3;

void write_color(std::ostream& out, color pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    static const interval intensity{0, 0.999};
    int ir = static_cast<int>(256 * intensity.clamp(r));
    int ig = static_cast<int>(256 * intensity.clamp(g));
    int ib = static_cast<int>(256 * intensity.clamp(b));

    out << ir << ' ' << ig << ' ' << ib << '\n';
}

