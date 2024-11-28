#pragma once

#include "interval.h"
#include "ray.h"
#include "vec3.h"

class aabb {
public:
    interval x, y, z;

    aabb() = default;
    aabb(const interval& x, const interval& y, const interval& z);
    aabb(const point3& a, const point3& b);
    aabb(const aabb& a, const aabb& b);

    const interval& axis_interval(int n) const;

    bool hit(const ray& r, interval ray_t) const;
    int longest_axis() const;

private:
    void pad_to_minimums();
};