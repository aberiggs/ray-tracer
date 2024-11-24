#include "color.h" // self

void write_color(std::ostream& out, color pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    static const interval intensity{0, 0.999};
    int ir = static_cast<int>(256 * intensity.clamp(r));
    int ig = static_cast<int>(256 * intensity.clamp(g));
    int ib = static_cast<int>(256 * intensity.clamp(b));

    out << ir << ' ' << ig << ' ' << ib << '\n';
}