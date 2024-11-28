#pragma once

#include "hittable.h"
#include "hittable_list.h"
#include "material.h"

#include <memory>

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

inline std::shared_ptr<hittable_list> box(const point3& a, const point3& b, material::ptr mat_ptr) {
    auto sides = std::make_shared<hittable_list>();

    // Construct the two opposite vertices with the minimum and maximum coordinates.
    auto min = point3(std::fmin(a.x(),b.x()), std::fmin(a.y(),b.y()), std::fmin(a.z(),b.z()));
    auto max = point3(std::fmax(a.x(),b.x()), std::fmax(a.y(),b.y()), std::fmax(a.z(),b.z()));

    auto dx = vec3(max.x() - min.x(), 0, 0);
    auto dy = vec3(0, max.y() - min.y(), 0);
    auto dz = vec3(0, 0, max.z() - min.z());

    sides->add(std::make_shared<quad>(point3(min.x(), min.y(), max.z()),  dx,  dy, mat_ptr)); // front
    sides->add(std::make_shared<quad>(point3(max.x(), min.y(), max.z()), -dz,  dy, mat_ptr)); // right
    sides->add(std::make_shared<quad>(point3(max.x(), min.y(), min.z()), -dx,  dy, mat_ptr)); // back
    sides->add(std::make_shared<quad>(point3(min.x(), min.y(), min.z()),  dz,  dy, mat_ptr)); // left
    sides->add(std::make_shared<quad>(point3(min.x(), max.y(), max.z()),  dx, -dz, mat_ptr)); // top
    sides->add(std::make_shared<quad>(point3(min.x(), min.y(), min.z()),  dx,  dz, mat_ptr)); // bottom

    return sides;
}