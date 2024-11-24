#pragma once

#include "hittable.h"

#include <memory>
#include <vector>

class hittable_list : public hittable {
public:
    std::vector<hittable::ptr> objects;

    hittable_list() = default;
    hittable_list(hittable::ptr object);

    void clear(); 

    void add(hittable::ptr object);

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override; 
};
