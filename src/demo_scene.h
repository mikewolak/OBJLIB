#ifndef DEMO_SCENE_H
#define DEMO_SCENE_H

#include "3d_engine.h"

void init_demo_scene(void);
void render_demo_scene(float rotation_x, float rotation_y, float rotation_z, float zoom_factor);
void cleanup_demo_scene(void);

#endif // DEMO_SCENE_H
