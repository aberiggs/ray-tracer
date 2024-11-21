#pragma once

#include "color.h"
#include "hittable.h"
#include "material.h"
#include "ray.h"

class camera {
public:
    double aspect_ratio      {1.0};
    int    image_width       {100};
    int    samples_per_pixel {10};
    int    max_depth         {10};

    double vfov     {90.0};
    point3 lookfrom {0, 0, 0};
    point3 lookat   {0, 0, -1};
    vec3   vup      {0, 1, 0};
    
    void render(const hittable& world) {
        initialize();

        std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

        int j {};
        for (j = 0; j < image_height; ++j) {
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0, 0, 0);
                for (int s = 0; s < samples_per_pixel; ++s) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, world, max_depth);
                }
                write_color(std::cout, pixel_samples_scale * pixel_color);
            }
            // Print progress
            constexpr char wheel[] = {'-', '\\', '|', '/'}; 
            std::clog << "\rProgress: " << j << "/" << image_height << " " << wheel[j % sizeof(wheel)] << std::flush;
        }
        std::clog << "\rProgress: " << j << "/" << image_height << "  " << std::flush;
        std::clog << "\nDone.\n";
    }

private:
    int image_height;
    double pixel_samples_scale;
    point3 center;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    vec3 u, v, w;

    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height; // Ensure height is at least 1

        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = lookfrom; 

        // Calculate viewport dimensions
        auto focal_length = 1.0;
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta / 2);
        auto viewport_height = 2 * h * focal_length;
        auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

        // Get orthonormal basis vectors for camera space
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Get vectors across the horizontal and down the vertical viewport edges
        auto viewport_u = viewport_width * u;
        auto viewport_v = viewport_height * -v;

        // Calculate horizontal and vertical delta vectors from pixel to pixel
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate position of upper left pixel
        auto viewport_upper_left = center - (focal_length * w) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    ray get_ray(int i, int j) const {
        auto offset = sample_square();
        auto pixel_sample = pixel00_loc + (i + offset.x()) * pixel_delta_u + (j + offset.y()) * pixel_delta_v;

        auto ray_direction = pixel_sample - center;

        return ray(center, ray_direction);
    }
    
    vec3 sample_square() const {
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }
    
    color ray_color(const ray& r, const hittable& world, int depth) const {
        if (depth <= 0) {
            return color(0, 0, 0);
        }

        hit_record rec;
        if (world.hit(r, {0.001, inf}, rec)) {
            ray scattered;
            color attenuation;
            if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
                return attenuation * ray_color(scattered, world, depth - 1);
            }
            return {0, 0, 0};
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }
};
