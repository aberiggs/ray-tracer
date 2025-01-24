#include "envs.h" // self

#include "color.h"
#include "constant_medium.h"
#include "material.h"
#include "quad.h"
#include "sphere.h"
#include "vec3.h"

using std::make_shared;
using std::shared_ptr;

hittable_list scene::spheres(camera& cam) {
    hittable_list world;
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.max_depth         = 50;
    cam.background        = color(0.70, 0.80, 1.00);

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;

    return world;
}

hittable_list scene::moving_spheres(camera& cam) {
    hittable_list world;
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto center2 = center + vec3(0, random_double(0, 0.5), 0);
                    world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color::random());
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.05);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.max_depth         = 50;
    cam.background        = color(0.70, 0.80, 1.00);

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;

    return world;
}

hittable_list scene::quads(camera& cam) {
    hittable_list world;

    // Materials
    auto left_red     = make_shared<lambertian>(color(1.0, 0.2, 0.2));
    auto back_green   = make_shared<lambertian>(color(0.2, 1.0, 0.2));
    auto right_blue   = make_shared<lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal   = make_shared<lambertian>(color(0.2, 0.8, 0.8));

    // Quads
    world.add(make_shared<quad>(point3(-3,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), left_red));
    world.add(make_shared<quad>(point3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    world.add(make_shared<quad>(point3( 3,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add(make_shared<quad>(point3(-2,-3, 5), vec3(4, 0, 0), vec3(0, 0,-4), lower_teal));

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.max_depth         = 50;
    cam.background        = color(0.70, 0.80, 1.00);

    cam.vfov     = 80;
    cam.lookfrom = point3(0,0,9);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    return world;
}

hittable_list scene::simple_light(camera& cam) {
    hittable_list world;

    auto obj_color = make_shared<solid_color>(color(1.0, 0.35, 0.24));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(color(0.8, 0.8, 0.8))));
    world.add(make_shared<sphere>(point3(0,2,0), 2, make_shared<lambertian>(obj_color)));

    auto difflight = make_shared<diffuse_light>(color(4,4,4));
    world.add(make_shared<quad>(point3(3, 1, -2), vec3(2, 0, 0), vec3(0, 2, 0), difflight));

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.max_depth         = 50;
    cam.background        = color(0.05, 0.05, 0.05);

    cam.vfov     = 20;
    cam.lookfrom = point3(26,3,6);
    cam.lookat   = point3(0,2,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    return world;
}

hittable_list scene::sphere_emit(camera& cam) {
    hittable_list world;

    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(color(0.8, 0.8, 0.8)))); // Ground
    
    auto plain = make_shared<lambertian>(color(1.0, 1.0, 1.0));
    world.add(make_shared<sphere>(point3(0,1,0), 1, plain));

    auto emission1 = make_shared<diffuse_light>(color(0.5, 0.5, 0.24));
    world.add(make_shared<sphere>(point3(-2,1,2), 1, emission1));
    auto emission2 = make_shared<diffuse_light>(color(0.5, 0.24, 0.5)); 
    world.add(make_shared<sphere>(point3(2,1,2), 1, emission2));


    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.max_depth         = 100;
    cam.background        = color(0.05, 0.05, 0.05);

    cam.vfov     = 20;
    cam.lookfrom = point3(0,3,26);
    cam.lookat   = point3(0,2,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    return world;
}

hittable_list scene::cornell_box(camera& cam) {
    hittable_list world;

    auto red   = make_shared<lambertian>(color(0.65, 0.05, 0.05));
    auto white = make_shared<lambertian>(color(0.9, 0.9, 0.9));
    auto green = make_shared<lambertian>(color(0.12, 0.45, 0.15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(make_shared<quad>(point3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), light));
    world.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), white));
    world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    shared_ptr<hittable> box1 = box(point3(0,0,0), point3(165, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));
    world.add(box1);

    shared_ptr<hittable> box2 = box(point3(0,0,0), point3(165, 165, 165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));
    world.add(box2);

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 600;
    cam.max_depth         = 50;
    cam.background        = color(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    return world;
}

hittable_list scene::cornell_box_alt1(camera& cam) {
    hittable_list world;

    auto red   = make_shared<lambertian>(color(0.65, 0.05, 0.05));
    auto white = make_shared<lambertian>(color(0.9, 0.9, 0.9));
    auto green = make_shared<lambertian>(color(0.12, 0.45, 0.15));
    auto light = make_shared<diffuse_light>(color(1, 1, 1));

    world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(make_shared<quad>(point3(477, 554, 477), vec3(-400,0,0), vec3(0,0,-400), light));    
    world.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), white));
    world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    shared_ptr<hittable> box1 = box(point3(0,0,0), point3(165, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));
    world.add(box1);

    shared_ptr<hittable> box2 = box(point3(0,0,0), point3(165, 165, 165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));
    world.add(box2);

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 600;
    cam.max_depth         = 50;
    cam.background        = color(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    return world;
}

hittable_list scene::checkered_spheres(camera& cam) {
    hittable_list world;

    auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));

    world.add(make_shared<sphere>(point3(0,-10, 0), 10, make_shared<lambertian>(checker)));
    world.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.max_depth         = 50;
    cam.background        = color(0.70, 0.80, 1.00);

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    return world;
}

hittable_list scene::pastel_box(camera& cam) {
    hittable_list world;

    auto white = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    auto red   = make_shared<lambertian>(0.6 * int_to_color(255,197,211));
    auto blue = make_shared<lambertian>(0.6 * int_to_color(179, 235, 242));
    auto white_light = make_shared<diffuse_light>(color(5, 5, 5));
    auto blue_light = make_shared<diffuse_light>(int_to_color(179, 235, 242), 1.0);
    auto red_light  = make_shared<diffuse_light>(int_to_color(255, 197, 211), 1.0);
    auto mirror = make_shared<metal>(color(0.7, 0.6, 0.5), 0.05);
    auto plain = make_shared<lambertian>(color(1.0, 1.0, 1.0));


    // Box
    world.add(make_shared<quad>(point3(800, 0, 0), vec3(0, 800, 0), vec3(0, 0, 800), blue));
    world.add(make_shared<quad>(point3(0, 0, 0), vec3(0, 800, 0), vec3(0, 0, 800), red));
    world.add(make_shared<quad>(point3(0, 0, 0), vec3(800, 0, 0), vec3(0, 0, 800), white));
    world.add(make_shared<quad>(point3(800, 800, 800), vec3(-800, 0, 0), vec3(0, 0, -800), white)); // Top
    world.add(make_shared<quad>(point3(500, 799, 500), vec3(-200,0,0), vec3(0,0,-200), white_light));    
    world.add(make_shared<quad>(point3(0, 0, 800), vec3(800, 0, 0), vec3(0, 800, 0), white));

    // Light sphere at top corners
    world.add(make_shared<sphere>(point3(700, 700, 700), 80, blue_light));
    world.add(make_shared<sphere>(point3(100, 700, 700), 80, red_light));

    // Rotated boxes on the ground
    shared_ptr<hittable> box1 = box(point3(0,0,0), point3(200, 200, 200), plain);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(500,0,400));
    world.add(box1);
    shared_ptr<hittable> box2 = box(point3(0,0,0), point3(200, 400, 200), plain);
    box2 = make_shared<rotate_y>(box2, 30);
    box2 = make_shared<translate>(box2, vec3(100,0,600));
    world.add(box2);

    // Add metal sphere on the ground
    world.add(make_shared<sphere>(point3(350, 100, 350), 100, mirror));

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 800;
    cam.max_depth         = 50;
    cam.background        = color(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point3(400, 400, -850);
    cam.lookat   = point3(400, 400, 0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    return world;
}

hittable_list scene::cornell_box_smoke(camera& cam) {
    hittable_list world;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));

    world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(make_shared<quad>(point3(113,554,127), vec3(330,0,0), vec3(0,0,305), light));
    world.add(make_shared<quad>(point3(0,555,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    shared_ptr<hittable> box1 = box(point3(0,0,0), point3(165,330,165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));

    shared_ptr<hittable> box2 = box(point3(0,0,0), point3(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));

    world.add(make_shared<constant_medium>(box1, 0.01, color(0,0,0)));
    world.add(make_shared<constant_medium>(box2, 0.01, color(1,1,1)));

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 600;
    cam.max_depth         = 50;
    cam.background        = color(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    return world;
}