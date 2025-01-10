#include "camera.h" // self

#include "hit_record.h"
#include "material.h"

#include <future>
#include <vector>

std::vector<color> camera::render(const hittable& world) {
    initialize();

    // Create output buffer
    std::vector<color> output_buffer(image_width * image_height);

    std::clog << "Starting render:\n";
    std::clog << "Image size: " << image_width << "x" << image_height << "\n";
    std::clog << "Samples per pixel: " << samples_per_pixel << "\n";
    std::clog << "Max depth: " << max_depth << "\n\n";

    int completed_rows {0};
    std::mutex completed_rows_mutex;

    std::vector<std::future<void>> futures;
    for (int j = 0; j < image_height; ++j) {
        if (j % 10 == 0) { // Prevent too many threads from being created at a time
            for (auto& future : futures) {
                future.wait();
            }
        }
        futures.push_back(std::async(std::launch::async, [this, &world, &output_buffer, &completed_rows, &completed_rows_mutex, j]() {
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0, 0, 0);
                for (int s = 0; s < samples_per_pixel; ++s) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, world, max_depth);
                }
                output_buffer[j * image_width + i] = pixel_color * pixel_samples_scale;
            }
            std::lock_guard<std::mutex> lock(completed_rows_mutex);
            ++completed_rows;
            std::clog << "\rProgress: " << completed_rows << "/" << image_height << std::flush;
        }));
    }

    for (auto& future : futures) {
        future.wait();
    }

    std::clog << "\n\nDone.\n";
    return output_buffer;
}

void camera::render_sample(const hittable& world, std::vector<color>& output_buffer) {
    std::vector<std::future<void>> futures;
    for (int j = 0; j < image_height; ++j) {
        if (j % 10 == 0) { // Prevent too many threads from being created at a time
            for (auto& future : futures) {
                future.wait();
            }
        }

        futures.push_back(std::async(std::launch::async, [this, &world, &output_buffer, j]() {
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0, 0, 0);
                    ray r = get_ray(i, j);
                    pixel_color = ray_color(r, world, max_depth);
                output_buffer[j * image_width + i] += pixel_color; // TODO: consider overflow?
            }
        }));
    }

    for (auto& future : futures) {
        future.wait();
    }
}

void camera::initialize() {
    image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height; // Ensure height is at least 1

    pixel_samples_scale = 1.0 / samples_per_pixel;

    center = lookfrom; 

    // Calculate viewport dimensions
    auto theta = degrees_to_radians(vfov);
    auto h = std::tan(theta / 2);
    auto viewport_height = 2 * h * focus_dist;
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
    auto viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Calculate the camera defocus disk basis vectors
    auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle) / 2);
    defocus_disk_u = u * defocus_radius;
    defocus_disk_v = v * defocus_radius;
}

ray camera::get_ray(int i, int j) const {
    auto offset = sample_square();
    auto pixel_sample = pixel00_loc + (i + offset.x()) * pixel_delta_u + (j + offset.y()) * pixel_delta_v;

    auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
    auto ray_direction = pixel_sample - ray_origin;
    auto ray_time = random_double();

    return ray(ray_origin, ray_direction, ray_time);
}

vec3 camera::sample_square() const {
    return vec3(random_double() - 0.5, random_double() - 0.5, 0);
}

// Return random point in the camera defocus disk
vec3 camera::defocus_disk_sample() const {
    auto p = random_in_unit_disk();
    return center + p.x() * defocus_disk_u + p.y() * defocus_disk_v;
}

color camera::ray_color(const ray& r, const hittable& world, int depth) const {
    if (depth <= 0) {
        return color(0, 0, 0);
    }

    hit_record rec;
    if (!world.hit(r, {0.001, inf}, rec))
        return background;
    
    ray scattered;
    color attenuation;
    color color_from_emission = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered)) 
        return color_from_emission;
    
    color color_from_scatter = attenuation * ray_color(scattered, world, depth - 1);
    return color_from_emission + color_from_scatter;
}