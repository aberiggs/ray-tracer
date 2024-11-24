#include "bvh_node.h"
#include "camera.h"
#include "envs.h"
#include "hittable_list.h"

using namespace std;

int main(int argc, char **argv) {
    camera cam;
    hittable_list world = scene_spheres(cam);
    // Optimize with BVH - TODO: Make argument
    world = hittable_list(make_shared<bvh_node>(world));

    // Set non-default camera settings
    for (int i = 0; i < argc; ++i) {
        if (std::string(argv[i]) == "-s" || std::string(argv[i]) == "--samples") {
            if (i + 1 >= argc) {
                std::cerr << "Error: Missing argument for samples per pixel\n";
                return 1;
            }
            cam.samples_per_pixel = std::stoi(argv[i + 1]);
        }
    }

    auto start = chrono::high_resolution_clock::now();

    std::vector<color> buff {cam.render(world)};
    std::cout << "P3\n" << cam.image_width << " " << buff.size() / cam.image_width << "\n255\n";
    for (const auto& c : buff) {
        write_color(std::cout, c);
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(end - start);
    std::clog << "\nRender time: " << duration.count() << " seconds\n";
}

