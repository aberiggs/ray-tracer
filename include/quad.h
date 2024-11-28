#pragma once

#include "hittable.h"
#include "material.h"

class quad : public hittable {
public:
    quad(const point3& Q, const vec3& u, const vec3& v, material::ptr mat_ptr);

    virtual void set_bounding_box(); 

    aabb bounding_box() const override;

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override;

    virtual bool is_interior(double a, double b, hit_record& rec) const;

private:
    point3 Q;
    vec3 u, v;
    vec3 w;
    material::ptr mat_ptr;
    aabb bbox;
    vec3 normal;
    double D;
};