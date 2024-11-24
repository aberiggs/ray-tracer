#pragma once

#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"


class bvh_node : public hittable {
public:
    bvh_node(hittable_list list) : bvh_node(list.objects, 0, list.objects.size()) {}

    bvh_node(std::vector<hittable::ptr>& objects, size_t start, size_t end);

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override;
    aabb bounding_box() const override;

private:
    hittable::ptr left;
    hittable::ptr right;
    aabb bbox;
};