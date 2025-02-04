#pragma once

#include "color.h"
#include "hittable.h"
#include "hit_record.h"
#include "ray.h"
#include "texture.h"

class material {
public:
    typedef std::shared_ptr<material> ptr;

    virtual ~material() = default;

    virtual color emitted(double, double, const point3&) const {
        return color(0, 0, 0);
    }

    virtual bool scatter(const ray&, const hit_record&, color&, ray& ) const {
        return false;
    };

};

class lambertian : public material {
public:
    lambertian(const color& a) : tex_ptr(std::make_shared<solid_color>(a)) {}
    lambertian(std::shared_ptr<texture> tex_ptr) : tex_ptr(tex_ptr) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        vec3 scatter_direction = rec.normal + random_unit_vector();

        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        scattered = ray(rec.p, scatter_direction, r_in.time());
        attenuation = tex_ptr->value(rec.u, rec.v, rec.p);
        return true;
    }

private:
    std::shared_ptr<texture> tex_ptr;
};

class metal : public material {
public:
    metal(const color& a, double f) : albedo(a), fuzz(f) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        reflected = unit_vector(reflected) + fuzz * random_unit_vector();
               
        scattered = ray(rec.p, reflected, r_in.time());
        attenuation = albedo;
        return dot(scattered.direction(), rec.normal) > 0;
    }

private:
    color albedo;
    double fuzz;
};

class dielectric : public material {
public:
    dielectric(double refraction_index) : refraction_index(refraction_index) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        attenuation = color(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face ? (1.0 / refraction_index) : refraction_index;

        vec3 unit_direction = unit_vector(r_in.direction());

        double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        vec3 direction;

        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
            // Must reflect
            direction = reflect(unit_direction, rec.normal);
        } else {
            // Can refract
            direction = refract(unit_direction, rec.normal, refraction_ratio);
        }

        scattered = ray(rec.p, direction, r_in.time()); 
        return true;
    }

private:
    double refraction_index;

    static double reflectance(double cosine, double refraction_index) {
        // Use Schlick's approximation for reflectance
        double r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};

class diffuse_light : public material {
public:
    diffuse_light(std::shared_ptr<texture> tex_ptr, double intensity = 1.0) : tex_ptr(tex_ptr), intensity(intensity) {}
    diffuse_light(const color& color, double intensity = 1.0) : tex_ptr(std::make_shared<solid_color>(color)), intensity(intensity) {}

    color emitted(double u, double v, const point3& p) const override {
        auto emission = tex_ptr->value(u, v, p) * intensity;
        return emission;
    }

private:
    std::shared_ptr<texture> tex_ptr;
    double intensity;
};

class isotropic : public material {
public:
    isotropic(std::shared_ptr<texture> tex_ptr) : tex_ptr(tex_ptr) {}
    isotropic(const color& albedo) : tex_ptr(std::make_shared<solid_color>(albedo)) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        scattered = ray(rec.p, random_unit_vector(), r_in.time());
        attenuation = tex_ptr->value(rec.u, rec.v, rec.p);
        return true;
    }

private:
    std::shared_ptr<texture> tex_ptr;
};