#pragma once

#include "camera.h"
#include "hittable_list.h"

// List of environments to generate to generate hittable_list's from

// Render high quality image of a sphere scene
hittable_list scene_spheres(camera& cam);

// Render image of a scene with moving spheres
hittable_list scene_moving_spheres(camera& cam);