#pragma once

#include "camera.h"
#include "hittable_list.h"

// List of environments to generate to generate hittable_list's from

namespace scene {
// Render high quality image of a sphere scene
hittable_list spheres(camera& cam);

// Render image of a scene with moving spheres
hittable_list moving_spheres(camera& cam);

// Render image of a scene with quadrilaterals
hittable_list quads(camera& cam);

// Render image of a scene with a simple light
hittable_list simple_light(camera& cam);

hittable_list sphere_emit(camera& cam);

// Render image of a scene with a cornell box
hittable_list cornell_box(camera& cam);

// Render image of a scene with a cornell box that has a bigger overhead light
hittable_list cornell_box_alt1(camera& cam);

// Render image of a scene with 2 checkered spheres
hittable_list checkered_spheres(camera& cam);

hittable_list pastel_box(camera& cam);

} // namespace scene