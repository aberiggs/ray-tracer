#pragma once

#include "ray.h"

#include <memory>

class hit_record {
public:
    point3 p;
    vec3 normal;
    double t;
    bool front_face;

    void set_face_normal(const ray& r, const vec3& outward_normal) {
        // NOTE: `outward_normal` is assumed to have unit length
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    typedef std::shared_ptr<hittable> ptr;

    virtual ~hittable() = default;

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};
