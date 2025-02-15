#pragma once

#include "color.h"
#include "hittable.h"
#include "ray.h"

#include <thread>

class camera {
public:
    double aspect_ratio      {1.0};
    int    image_width       {100}; // TODO: should be private - can't be updated during a render
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
    bool get_should_render() const { return should_render; }
    bool get_is_rendering() const { return is_rendering; }
    long get_render_time() const { return render_time; }


    // Halt rendering by killing the render thread + all workers
    void stop(bool join = true);

    // Reset the camera to its initial state
    void reset();

    // Initialize the camera with the given parameters
    void load_scene(std::shared_ptr<hittable> scene_ptr);

private:
    std::atomic<bool> should_render {false};
    std::atomic<bool> is_rendering {false};
    // Note - `num_samples` may not always be accurate for a given pixel due to the way I handle async rendering.
    std::atomic<long> num_samples {0}; 
    // Time spent rendering in milliseconds
    std::atomic<long> render_time {0};
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
    std::vector<color> render_sample_async(int num_workers);

    // i = col, j = row
    ray get_ray(int i, int j) const;
    
    vec3 sample_square() const; 

    // Return random point in the camera defocus disk
    vec3 defocus_disk_sample() const; 
    
    color ray_color(const ray& r, int depth) const;
};
