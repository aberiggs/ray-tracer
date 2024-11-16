#pragma once

#include "hittable.h"

#include <memory>
#include <vector>

class hittable_list : public hittable {
public:
    std::vector<hittable::ptr> objects;

    hittable_list() {};
    hittable_list(hittable::ptr object) { add(object); }

    void clear() {objects.clear(); }

    void add(hittable::ptr object) { objects.push_back(object); }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;
        for (const auto& object : objects) {
            if (object->hit(r, {ray_t.min, closest_so_far}, temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
        return hit_anything;
    }
};
