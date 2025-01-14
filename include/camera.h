#pragma once

#include "color.h"
#include "hittable.h"
#include "ray.h"

#include <thread>

class camera {
public:
    double aspect_ratio      {1.0};
    int    image_width       {100};
    int    samples_per_pixel {10};
    int    max_depth         {10};
    color  background        {0, 0, 0};

    double vfov     {90.0};
    point3 lookfrom {0, 0, 0};
    point3 lookat   {0, 0, -1};
    vec3   vup      {0, 1, 0};

    double defocus_angle {0};
    double focus_dist {10};
    
    std::vector<color> render(const hittable& world);

    void render_async(const std::shared_ptr<hittable> scene); // Continuously render samples
    std::vector<color> render_sample(const hittable& scene);

    void render_data(std::vector<unsigned char>& pixel_data, int& width, int& height);

    long get_num_samples() const { return num_samples; }

    void stop();

    void initialize();

private:
    std::atomic<bool> should_render {false};
    std::atomic<long> num_samples {0};
    std::vector<std::atomic<color>> pixel_samples {}; // 2D array of pixel samples
    std::thread render_thread {};

    int    image_height;
    double pixel_samples_scale;
    point3 center;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    vec3 u, v, w;
    vec3 defocus_disk_v;
    vec3 defocus_disk_u;

    ray get_ray(int i, int j) const;
    
    vec3 sample_square() const; 

    // Return random point in the camera defocus disk
    vec3 defocus_disk_sample() const; 
    
    color ray_color(const ray& r, const hittable& world, int depth) const;
};
