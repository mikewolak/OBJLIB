#include "3d_engine.h"
#include <math.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>

#define COLOR DEFAULT_COLOR


static int screen_width, screen_height;
static void (*put_pixel_callback)(int x, int y, uint32_t color);

void set_put_pixel_callback(void (*callback)(int x, int y, uint32_t color)) {
    put_pixel_callback = callback;
}

void init_3d_engine(int width, int height) {
    screen_width = width;
    screen_height = height;
}

void resize_3d_engine(int width, int height) {
    screen_width = width;
    screen_height = height;
}

int get_screen_width(void) {
    return screen_width;
}

int get_screen_height(void) {
    return screen_height;
}

void engine_put_pixel(int x, int y, uint32_t color) {
    if (put_pixel_callback) {
        put_pixel_callback(x, y, color);
    }
}


static void draw_line(int x1, int y1, int x2, int y2, int thick) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        int thickness = thick ? 1 : 0;
        for (int i = -thickness; i <= thickness; i++) {
            for (int j = -thickness; j <= thickness; j++) {
                int px = x1 + i;
                int py = y1 + j;
                if (px >= 0 && px < screen_width && py >= 0 && py < screen_height) {
                    put_pixel_callback(px, py, COLOR);
                }
            }
        }

        if (x1 == x2 && y1 == y2) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

static Vector3 rotate_point(Vector3 p, float angle_x, float angle_y, float angle_z) {
    Vector3 result = p;

    // Rotate around X-axis
    float cos_x = cosf(angle_x);
    float sin_x = sinf(angle_x);
    float y = result.y;
    result.y = y * cos_x - result.z * sin_x;
    result.z = y * sin_x + result.z * cos_x;

    // Rotate around Y-axis
    float cos_y = cosf(angle_y);
    float sin_y = sinf(angle_y);
    float x = result.x;
    result.x = x * cos_y + result.z * sin_y;
    result.z = -x * sin_y + result.z * cos_y;

    // Rotate around Z-axis
    float cos_z = cosf(angle_z);
    float sin_z = sinf(angle_z);
    x = result.x;
    y = result.y;
    result.x = x * cos_z - y * sin_z;
    result.y = x * sin_z + y * cos_z;

    return result;
}

void render_mesh(Mesh *mesh, float rotation_x, float rotation_y, float rotation_z, float zoom_factor, int thick_lines) {
    for (int i = 0; i < mesh->num_faces; i++) {
        Face face = mesh->faces[i];
        Vector3 v1 = rotate_point(mesh->vertices[face.v1], rotation_x, rotation_y, rotation_z);
        Vector3 v2 = rotate_point(mesh->vertices[face.v2], rotation_x, rotation_y, rotation_z);
        Vector3 v3 = rotate_point(mesh->vertices[face.v3], rotation_x, rotation_y, rotation_z);

        // Apply zoom factor
        v1.x *= zoom_factor; v1.y *= zoom_factor; v1.z *= zoom_factor;
        v2.x *= zoom_factor; v2.y *= zoom_factor; v2.z *= zoom_factor;
        v3.x *= zoom_factor; v3.y *= zoom_factor; v3.z *= zoom_factor;

        // Simple perspective projection
        float z_offset = 5.0f;
        int x1 = (int)((v1.x / (v1.z + z_offset)) * screen_width + screen_width / 2);
        int y1 = (int)((-v1.y / (v1.z + z_offset)) * screen_height + screen_height / 2);  // Note the negative sign
        int x2 = (int)((v2.x / (v2.z + z_offset)) * screen_width + screen_width / 2);
        int y2 = (int)((-v2.y / (v2.z + z_offset)) * screen_height + screen_height / 2);  // Note the negative sign
        int x3 = (int)((v3.x / (v3.z + z_offset)) * screen_width + screen_width / 2);
        int y3 = (int)((-v3.y / (v3.z + z_offset)) * screen_height + screen_height / 2);  // Note the negative sign

        draw_line(x1, y1, x2, y2, thick_lines);
        draw_line(x2, y2, x3, y3, thick_lines);
        draw_line(x3, y3, x1, y1, thick_lines);
    }
}

void center_mesh(Mesh *mesh) {
    Vector3 center = {0};
    for (int i = 0; i < mesh->num_vertices; i++) {
        center.x += mesh->vertices[i].x;
        center.y += mesh->vertices[i].y;
        center.z += mesh->vertices[i].z;
    }
    center.x /= mesh->num_vertices;
    center.y /= mesh->num_vertices;
    center.z /= mesh->num_vertices;

    for (int i = 0; i < mesh->num_vertices; i++) {
        mesh->vertices[i].x -= center.x;
        mesh->vertices[i].y -= center.y;
        mesh->vertices[i].z -= center.z;
    }
}

void create_cube(Mesh *mesh) {
    mesh->num_vertices = 8;
    mesh->num_faces = 12;
    mesh->vertices = malloc(sizeof(Vector3) * mesh->num_vertices);
    mesh->faces = malloc(sizeof(Face) * mesh->num_faces);

    // Cube vertices
    mesh->vertices[0] = (Vector3){-0.5f, -0.5f, -0.5f};
    mesh->vertices[1] = (Vector3){ 0.5f, -0.5f, -0.5f};
    mesh->vertices[2] = (Vector3){ 0.5f,  0.5f, -0.5f};
    mesh->vertices[3] = (Vector3){-0.5f,  0.5f, -0.5f};
    mesh->vertices[4] = (Vector3){-0.5f, -0.5f,  0.5f};
    mesh->vertices[5] = (Vector3){ 0.5f, -0.5f,  0.5f};
    mesh->vertices[6] = (Vector3){ 0.5f,  0.5f,  0.5f};
    mesh->vertices[7] = (Vector3){-0.5f,  0.5f,  0.5f};

    // Cube faces
    mesh->faces[0] = (Face){0, 1, 2};
    mesh->faces[1] = (Face){0, 2, 3};
    mesh->faces[2] = (Face){1, 5, 6};
    mesh->faces[3] = (Face){1, 6, 2};
    mesh->faces[4] = (Face){5, 4, 7};
    mesh->faces[5] = (Face){5, 7, 6};
    mesh->faces[6] = (Face){4, 0, 3};
    mesh->faces[7] = (Face){4, 3, 7};
    mesh->faces[8] = (Face){3, 2, 6};
    mesh->faces[9] = (Face){3, 6, 7};
    mesh->faces[10] = (Face){4, 5, 1};
    mesh->faces[11] = (Face){4, 1, 0};
}

void free_mesh(Mesh *mesh) {
    free(mesh->vertices);
    free(mesh->faces);
}

float fit_mesh_to_screen(Mesh *mesh, int screen_width, int screen_height) {
    float min_x = FLT_MAX, max_x = -FLT_MAX;
    float min_y = FLT_MAX, max_y = -FLT_MAX;
    float min_z = FLT_MAX, max_z = -FLT_MAX;

    for (int i = 0; i < mesh->num_vertices; i++) {
        Vector3 v = mesh->vertices[i];
        min_x = fminf(min_x, v.x);
        max_x = fmaxf(max_x, v.x);
        min_y = fminf(min_y, v.y);
        max_y = fmaxf(max_y, v.y);
        min_z = fminf(min_z, v.z);
        max_z = fmaxf(max_z, v.z);
    }

    float width = max_x - min_x;
    float height = max_y - min_y;
    float depth = max_z - min_z;

    float max_dimension = fmaxf(width, fmaxf(height, depth));
    
    // Base zoom factor
    float zoom_factor = 0.75f;

    // Slight adjustment based on mesh size relative to screen
    float screen_dimension = fminf(screen_width, screen_height);
    float size_ratio = max_dimension / screen_dimension;

    // Adjust zoom factor slightly based on size ratio
    zoom_factor *= (1.0f + (size_ratio - 1.0f) * 0.1f);

    return zoom_factor;
}

void translate_mesh(Mesh *mesh, float dx, float dy, float dz) {
    for (int i = 0; i < mesh->num_vertices; i++) {
        mesh->vertices[i].x += dx;
        mesh->vertices[i].y += dy;
        mesh->vertices[i].z += dz;
    }
}

Mesh copy_mesh(const Mesh* original) {
    Mesh copy;
    copy.num_vertices = original->num_vertices;
    copy.num_faces = original->num_faces;
    copy.vertices = malloc(sizeof(Vector3) * copy.num_vertices);
    copy.faces = malloc(sizeof(Face) * copy.num_faces);
    memcpy(copy.vertices, original->vertices, sizeof(Vector3) * copy.num_vertices);
    memcpy(copy.faces, original->faces, sizeof(Face) * copy.num_faces);
    return copy;
}

void scale_mesh(Mesh *mesh, float scale) {
    for (int i = 0; i < mesh->num_vertices; i++) {
        mesh->vertices[i].x *= scale;
        mesh->vertices[i].y *= scale;
        mesh->vertices[i].z *= scale;
    }
}

