#include "bvh_node.h"

#include "utils.h"

#include <algorithm>

bvh_node::bvh_node(std::vector<hittable::ptr>& objects, size_t start, size_t end) {
    bbox = aabb(interval::empty(), interval::empty(), interval::empty());
    for (size_t i = start; i < end; ++i) {
        bbox = aabb(bbox, objects[i]->bounding_box());
    }

    int axis = bbox.longest_axis();

    auto comparator = [axis](const hittable::ptr& a, const hittable::ptr& b) {
        return a->bounding_box().axis_interval(axis).min < b->bounding_box().axis_interval(axis).min;
    };

    size_t object_span = end - start;

    if (object_span == 1) {
        left = right = objects[start];
    } else if (object_span == 2) {
        if (comparator(objects[start], objects[start + 1])) {
            left = objects[start];
            right = objects[start + 1];
        } else {
            left = objects[start + 1];
            right = objects[start];
        }
    } else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + object_span / 2;
        left = std::make_shared<bvh_node>(objects, start, mid);
        right = std::make_shared<bvh_node>(objects, mid, end);
    }
}

bool bvh_node::hit(const ray& r, interval ray_t, hit_record& rec) const {
    if (!bbox.hit(r, ray_t)) {
        return false;
    }

    bool hit_left = left->hit(r, ray_t, rec);
    bool hit_right = right->hit(r, interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);

    return hit_left || hit_right;
}

aabb bvh_node::bounding_box() const {
    return bbox;
}
