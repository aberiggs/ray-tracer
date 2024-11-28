#pragma once

#include "ray.h"
#include "vec3.h"

#include <memory>

class material;

class hit_record {
public:
    point3 p;
    vec3 normal;
    std::shared_ptr<material> mat_ptr;
    double t;
    double u;
    double v;
    bool front_face;

    void set_face_normal(const ray& r, const vec3& outward_normal);
};