#include "camera.h" // self

#include "hit_record.h"
#include "material.h"

#include <future>
#include <vector>

void camera::render_async(int num_workers) {
    // TODO: Lock

    should_render = false;
    // Join the previous thread if it's still running
    if (render_thread.joinable()) {
        render_thread.join();
    }

    should_render = true;
    // Start rendering asynchronously
    render_thread = std::thread([this, num_workers]() {
        std::vector<std::thread> workers;

        if (!scene_ptr) {
            // Nothing to render
            return;
        }

        for (int w = 0; w < num_workers; ++w) {
            workers.push_back(std::thread([this]() {
                while (should_render) {
                    auto sample = render_sample();
                    for (int i = 0; i < image_width * image_height; ++i) {
                        pixel_samples[i] = pixel_samples[i] + sample[i];
                    }
                    ++num_samples;
                }
            }));
        }

        for (auto& worker : workers) {
            if (worker.joinable())
                worker.join();
        }
    });
}

std::vector<color> camera::render_sample() {
    std::vector<color> sample(image_width * image_height);

    for (int i = 0; i < image_height; ++i) {
        for (int j = 0; j < image_width; ++j) {
            color pixel_color(0, 0, 0);
                ray r = get_ray(j, i);
                pixel_color = ray_color(r, max_depth);
            sample[i * image_width + j] = pixel_color;
        }
    }

    return sample;
}

void camera::render_data(std::vector<unsigned char>& pixel_data) {
    // Lock the scene mutex - we're going to be reading from the scene
    std::lock_guard<std::mutex> guard(scene_mutex);

    pixel_data = std::vector<unsigned char>(image_width * image_height * 3);

    if (num_samples == 0) {
        for (int i = 0; i < image_width * image_height; ++i) {
            pixel_data.push_back(0);
            pixel_data.push_back(0);
            pixel_data.push_back(0);
        }
    } else {
        // Iterate through pixel samples and average them
        for (int i = 0; i < image_height; ++i) {
            for (int j = 0; j < image_width; ++j) {
                auto pixel = pixel_samples[i * image_width + j].load() / num_samples;
                pixel_data[(i * image_width + j) * 3] = get_color_component(pixel, 0);
                pixel_data[(i * image_width + j) * 3 + 1] = get_color_component(pixel, 1);
                pixel_data[(i * image_width + j) * 3 + 2] = get_color_component(pixel, 2);
            }
        }
    }
}

void camera::stop() {
    should_render = false;
    // if (render_thread.joinable()) {
        // render_thread.join();
    // }
}

void camera::reset() {
    // Kill the render thread if it's still running
    stop();

    pixel_samples = std::vector<std::atomic<color>>(image_width * image_height);
    num_samples = 0;
    for (int i = 0; i < image_width * image_height; ++i) {
        pixel_samples[i] = color(0, 0, 0);
    }
}

void camera::load_scene(std::shared_ptr<hittable> scene_ptr) {

    // We're going to be changing the scene, so lock the mutex
    std::lock_guard<std::mutex> guard(scene_mutex);

    this->scene_ptr = scene_ptr;

    image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height; // Ensure height is at least 1

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

    // Reset the camera with the new scene
    reset();

    std::cout << "Finished loading scene" << std::endl;
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

color camera::ray_color(const ray& r, int depth) const {
    if (depth <= 0) {
        return color(0, 0, 0);
    }

    hit_record rec;
    if (!scene_ptr->hit(r, {0.001, inf}, rec))
        return background;
    
    ray scattered;
    color attenuation;
    color color_from_emission = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered)) 
        return color_from_emission;
    
    color color_from_scatter = attenuation * ray_color(scattered, depth - 1);

    return color_from_emission + color_from_scatter;
}