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