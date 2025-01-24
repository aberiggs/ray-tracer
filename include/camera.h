#pragma once

#include "color.h"
#include "hittable.h"
#include "ray.h"

#include <thread>

class camera {
public:
    double aspect_ratio      {1.0};
    int    image_width       {100};
    int    max_depth         {10};
    color  background        {0, 0, 0};

    double vfov     {90.0};
    point3 lookfrom {0, 0, 0};
    point3 lookat   {0, 0, -1};
    vec3   vup      {0, 1, 0};

    double defocus_angle {0};
    double focus_dist {10};

    // Continuously render samples on `workers` threads
    void render_async(int num_workers = 1);

    // Append normalized pixel data to `pixel_data`
    void render_data(std::vector<unsigned char>& pixel_data);

    // Get the number of samples rendered
    long get_num_samples() const { return num_samples; }

    int get_image_width() const { return image_width; }
    int get_image_height() const { return image_height; }
    bool is_rendering() const { return should_render; }

    // Halt rendering by killing the render thread + all workers
    void stop();

    // Reset the camera to its initial state
    void reset();

    // Initialize the camera with the given parameters
    void load_scene(std::shared_ptr<hittable> scene_ptr);

private:
    std::atomic<bool> should_render {false};
    std::atomic<long> num_samples {0};
    std::vector<std::atomic<color>> pixel_samples {}; // 2D array of pixel samples
    std::thread render_thread {};
    std::mutex scene_mutex {};

    std::shared_ptr<hittable> scene_ptr {nullptr};
    int image_height;
    point3 center;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    vec3 u, v, w;
    vec3 defocus_disk_v;
    vec3 defocus_disk_u;

    // Render a single sample of the scene
    std::vector<color> render_sample();

    ray get_ray(int i, int j) const;
    
    vec3 sample_square() const; 

    // Return random point in the camera defocus disk
    vec3 defocus_disk_sample() const; 
    
    color ray_color(const ray& r, int depth) const;
};
