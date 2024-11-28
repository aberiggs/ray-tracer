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

class translate : public hittable {
public:
    translate(std::shared_ptr<hittable> obj_ptr, const vec3& offset) : obj_ptr(obj_ptr), offset(offset) {
        bbox = obj_ptr->bounding_box() + offset;
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        ray offset_r{r.origin() - offset, r.direction(), r.time()}; 
        if (!obj_ptr->hit(offset_r, ray_t, rec))
            return false;

        rec.p += offset;

        return true;
    }

    aabb bounding_box() const override {
        return bbox;
    }

private:
    std::shared_ptr<hittable> obj_ptr;
    vec3 offset;
    aabb bbox;
};

class rotate_y : public hittable {
public:
    rotate_y(std::shared_ptr<hittable> object, double angle) : obj_ptr(object) {
        auto radians = degrees_to_radians(angle);
        sin_theta = sin(radians);
        cos_theta = cos(radians);
        bbox = obj_ptr->bounding_box();

        point3 min {+inf, +inf, +inf};
        point3 max {-inf, -inf, -inf};

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    auto x = i * bbox.x.max + (1 - i) * bbox.x.min;
                    auto y = j * bbox.y.max + (1 - j) * bbox.y.min;
                    auto z = k * bbox.z.max + (1 - k) * bbox.z.min;

                    auto new_x = cos_theta * x + sin_theta * z;
                    auto new_z = -sin_theta * x + cos_theta * z;

                    vec3 tester(new_x, y, new_z);

                    for (int c = 0; c < 3; c++) {
                        min[c] = std::min(min[c], tester[c]);
                        max[c] = std::max(max[c], tester[c]);
                    }
                }
            }
        }
        bbox = aabb(min, max);
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        auto origin = point3(
            cos_theta * r.origin().x() - sin_theta * r.origin().z(),
            r.origin().y(),
            sin_theta * r.origin().x() + cos_theta * r.origin().z()
        );

        auto direction = vec3(
            cos_theta * r.direction().x() - sin_theta * r.direction().z(),
            r.direction().y(),
            sin_theta * r.direction().x() + cos_theta * r.direction().z()
        );

        ray rotated_r {origin, direction, r.time()};
        if (!obj_ptr->hit(rotated_r, ray_t, rec))
            return false;

        rec.p = point3(
            cos_theta * rec.p.x() + sin_theta * rec.p.z(),
            rec.p.y(),
            -sin_theta * rec.p.x() + cos_theta * rec.p.z()
        );

        rec.normal = vec3(
            cos_theta * rec.normal.x() + sin_theta * rec.normal.z(),
            rec.normal.y(),
            -sin_theta * rec.normal.x() + cos_theta * rec.normal.z()
        );

        return true;
    }

    aabb bounding_box() const override {
        return bbox;
    }

private:
    std::shared_ptr<hittable> obj_ptr;
    double sin_theta;
    double cos_theta;
    aabb bbox;
};