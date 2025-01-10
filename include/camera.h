#pragma once

#include "color.h"
#include "hittable.h"
#include "ray.h"

class camera {
public:
    double aspect_ratio      {1.0};
    int    image_width       {100};
    int    image_height;
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
    void render_sample(const hittable& world, std::vector<color>& output_buffer);

    void initialize();

private:
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
