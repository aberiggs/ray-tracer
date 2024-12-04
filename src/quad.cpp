#include "quad.h" // self

quad::quad(const point3& Q, const vec3& u, const vec3& v, material::ptr mat_ptr) : Q(Q), u(u), v(v), mat_ptr(mat_ptr) {
    auto n = cross(u, v);
    normal = unit_vector(n);
    D = dot(normal, Q);
    w = n / dot(n,n);

    set_bounding_box();
}

void quad::set_bounding_box() {
    auto bbox_diagonal1 = aabb(Q, Q + u + v);
    auto bbox_diagonal2 = aabb(Q + u, Q + v);
    bbox = aabb(bbox_diagonal1, bbox_diagonal2);
}

aabb quad::bounding_box() const {
    return bbox;
}

bool quad::hit(const ray& r, interval ray_t, hit_record& rec) const {
    auto denom = dot(normal, r.direction());

    // Not hit if the ray is parallel to plane
    if (std::fabs(denom) < 1e-8) {
        return false;
    }

    auto t = (D - dot(normal, r.origin())) / denom;
    if (!ray_t.surrounds(t)) {
        return false;
    }

    auto intersection = r.at(t);
    vec3 planar_hitpt_vector = intersection - Q;
    auto alpha = dot(w, cross(planar_hitpt_vector, v));
    auto beta = dot(w, cross(u, planar_hitpt_vector));

    if (!is_interior(alpha, beta, rec)) {
        return false;
    }

    rec.t = t;
    rec.p = intersection;
    rec.mat_ptr = mat_ptr;
    rec.set_face_normal(r, normal);

    return true;
}

bool quad::is_interior(double a, double b, hit_record& rec) const {
    interval unit_interval {0, 1}; 
    if (!unit_interval.surrounds(a) || !unit_interval.surrounds(b)) {
        return false;
    }

    rec.u = a;
    rec.v = b;
    return true;
}