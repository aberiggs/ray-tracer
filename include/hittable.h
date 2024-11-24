#pragma once

#include "aabb.h"
#include "hit_record.h"
#include "interval.h"
#include "ray.h"

#include <memory>

class hittable {
public:
    typedef std::shared_ptr<hittable> ptr;

    virtual ~hittable() = default;

    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;

    virtual aabb bounding_box() const = 0;
};
