#ifndef _3D_ENGINE_H
#define _3D_ENGINE_H

#include <stdint.h>

#define COLOR_RED     0xFF0000
#define COLOR_GREEN   0x00FF00
#define COLOR_BLUE    0x0000FF
#define COLOR_WHITE   0xFFFFFF
#define COLOR_BLACK   0x000000
#define COLOR_YELLOW  0xFFFF00
#define COLOR_CYAN    0x00FFFF
#define COLOR_MAGENTA 0xFF00FF
#define COLOR_GRAY    0x808080
#define COLOR_ORANGE  0xFFA500
#define COLOR_PURPLE  0x800080
#define COLOR_BROWN   0xA52A2A
#define COLOR_PINK    0xFFC0CB
#define COLOR_LIME    0x32CD32
#define COLOR_TEAL    0x008080
#define COLOR_NAVY    0x000080

#define DEFAULT_COLOR COLOR_GREEN

typedef struct {
    float x, y, z;
} Vector3;

typedef struct {
    int v1, v2, v3;
} Face;

typedef struct {
    Vector3 *vertices;
    Face *faces;
    int num_vertices;
    int num_faces;
} Mesh;

void init_3d_engine(int width, int height);
void resize_3d_engine(int width, int height);
void render_mesh(Mesh *mesh, float rotation_x, float rotation_y, float rotation_z, float zoom_factor, int thick_lines);
void center_mesh(Mesh *mesh);
void create_cube(Mesh *mesh);
void free_mesh(Mesh *mesh);
float fit_mesh_to_screen(Mesh *mesh, int screen_width, int screen_height);
void set_put_pixel_callback(void (*callback)(int x, int y, uint32_t color));
Mesh copy_mesh(const Mesh* original);
void translate_mesh(Mesh *mesh, float dx, float dy, float dz);
void scale_mesh(Mesh *mesh, float scale);
int get_screen_width(void);
int get_screen_height(void);
void engine_put_pixel(int x, int y, uint32_t color);

#endif
