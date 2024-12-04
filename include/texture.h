#pragma once

#include "color.h"
#include "vec3.h"

class texture {
public:
    virtual ~texture() = default;

    virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture {
public:
    solid_color(const color& albedo) : albedo(albedo) {}

    solid_color(double r, double g, double b) : solid_color(color(r, g, b)) {}

    color value(double, double, const point3&) const override {
        return albedo;
    }

private:
    color albedo;
};

class checker_texture : public texture {
public:
    checker_texture(double scale, std::shared_ptr<texture> even, std::shared_ptr<texture> odd) : inv_scale(1.0 / scale), even(even), odd(odd) {}

    checker_texture(double scale, const color& c1, const color& c2) : inv_scale(1.0 / scale), even(std::make_shared<solid_color>(c1)), odd(std::make_shared<solid_color>(c2)) {}

    color value(double u, double v, const point3& p) const override {
        auto xInt = static_cast<int>(inv_scale * p.x());
        auto yInt = static_cast<int>(inv_scale * p.y());
        auto zInt = static_cast<int>(inv_scale * p.z());
        
        bool isEven = (xInt + yInt + zInt) % 2 == 0;

        return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    }

private:
    double inv_scale;
    std::shared_ptr<texture> even;
    std::shared_ptr<texture> odd;
};