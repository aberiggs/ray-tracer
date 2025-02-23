#include "color.h" // self

int get_color_component(color pixel_color, int component) {
    // TODO: Clean up

    static const interval intensity{0, 0.999};

    double pixel_val;

    if (component == 0) {
        // r
        pixel_val = pixel_color.x();
    } else if (component == 1) {
        // g
        pixel_val = pixel_color.y();
    } else if (component == 2) {
        // b
        pixel_val = pixel_color.z();
    } else {
        return 0;
    }

    pixel_val = linear_to_gamma(pixel_val);
    int val = static_cast<int>(256 * intensity.clamp(pixel_val));
    return val;
}

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