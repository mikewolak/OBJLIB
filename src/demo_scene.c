#include "demo_scene.h"
#include "3d_engine.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "cow.h"
#include "homer.h"

#define MAX_LASERS 5
#define LASER_LIFETIME 100
#define LASER_COLOR 0xFF0000  // Red color for lasers

static Mesh sphere;
static Mesh spaceship1;
static Mesh spaceship2;
static Mesh spaceship3;
static Mesh enterprise;  // Add this line

static float demo_time = 0.0f;
static float sphere_rotation = 0.0f;

typedef struct {
    Vector3 start;
    Vector3 end;
    int lifetime;
} Laser;

//static Laser lasers[MAX_LASERS];
//static int num_lasers = 0;


static void project_point(Vector3 point, int *x, int *y) {
    //float z_offset = 5.0f;
    float scale = fminf(get_screen_width(), get_screen_height()) / (2.0f * 8.5f);  // 8.5f matches the orbit_radius
    *x = (int)((point.x * scale) + get_screen_width() / 2);
    *y = (int)((-point.y * scale) + get_screen_height() / 2);

    // Clamp values to screen bounds
    *x = (*x < 0) ? 0 : (*x >= get_screen_width() ? get_screen_width() - 1 : *x);
    *y = (*y < 0) ? 0 : (*y >= get_screen_height() ? get_screen_height() - 1 : *y);
}


void draw_position_marker(Vector3 position, uint32_t color) {
    int x, y;
    project_point(position, &x, &y);
    
    for (int i = -3; i <= 3; i++) {
        engine_put_pixel(x + i, y, color);
        engine_put_pixel(x, y + i, color);
    }
}

static void create_enterprise(Mesh *mesh) {
    mesh->num_vertices = 11;
    mesh->num_faces = 16;
    mesh->vertices = malloc(sizeof(Vector3) * mesh->num_vertices);
    mesh->faces = malloc(sizeof(Face) * mesh->num_faces);

    // Saucer section
    mesh->vertices[0] = (Vector3){0, 0.1f, 0};     // Top center
    mesh->vertices[1] = (Vector3){-0.5f, 0, 0.3f}; // Front left
    mesh->vertices[2] = (Vector3){0.5f, 0, 0.3f};  // Front right
    mesh->vertices[3] = (Vector3){-0.5f, 0, -0.3f};// Rear left
    mesh->vertices[4] = (Vector3){0.5f, 0, -0.3f}; // Rear right

    // Hull
    mesh->vertices[5] = (Vector3){0, -0.1f, -0.6f};// Rear
    mesh->vertices[6] = (Vector3){0, -0.2f, 0.2f}; // Front bottom

    // Nacelles
    mesh->vertices[7] = (Vector3){-0.7f, -0.1f, -0.4f}; // Left nacelle front
    mesh->vertices[8] = (Vector3){-0.7f, -0.1f, -0.8f}; // Left nacelle rear
    mesh->vertices[9] = (Vector3){0.7f, -0.1f, -0.4f};  // Right nacelle front
    mesh->vertices[10] = (Vector3){0.7f, -0.1f, -0.8f}; // Right nacelle rear

    // Saucer faces
    mesh->faces[0] = (Face){0, 1, 2};
    mesh->faces[1] = (Face){0, 3, 1};
    mesh->faces[2] = (Face){0, 4, 3};
    mesh->faces[3] = (Face){0, 2, 4};
    mesh->faces[4] = (Face){1, 3, 4};
    mesh->faces[5] = (Face){1, 4, 2};

    // Hull faces
    mesh->faces[6] = (Face){1, 6, 2};
    mesh->faces[7] = (Face){3, 5, 4};
    mesh->faces[8] = (Face){1, 3, 5};
    mesh->faces[9] = (Face){1, 5, 6};
    mesh->faces[10] = (Face){2, 6, 5};
    mesh->faces[11] = (Face){2, 5, 4};

    // Nacelle faces
    mesh->faces[12] = (Face){7, 8, 5};
    mesh->faces[13] = (Face){9, 5, 10};
    mesh->faces[14] = (Face){7, 5, 6};
    mesh->faces[15] = (Face){9, 6, 5};
}

static void create_spaceship(Mesh *mesh) {
    mesh->num_vertices = 5;
    mesh->num_faces = 6;
    mesh->vertices = malloc(sizeof(Vector3) * mesh->num_vertices);
    mesh->faces = malloc(sizeof(Face) * mesh->num_faces);

    mesh->vertices[0] = (Vector3){0, 0.5f, 0};
    mesh->vertices[1] = (Vector3){-0.5f, -0.5f, 0.5f};
    mesh->vertices[2] = (Vector3){0.5f, -0.5f, 0.5f};
    mesh->vertices[3] = (Vector3){-0.5f, -0.5f, -0.5f};
    mesh->vertices[4] = (Vector3){0.5f, -0.5f, -0.5f};

    mesh->faces[0] = (Face){0, 1, 2};
    mesh->faces[1] = (Face){0, 3, 1};
    mesh->faces[2] = (Face){0, 2, 4};
    mesh->faces[3] = (Face){0, 4, 3};
    mesh->faces[4] = (Face){1, 3, 4};
    mesh->faces[5] = (Face){1, 4, 2};
}

static void create_sphere(Mesh *mesh, int subdivisions) {
    int num_vertices = (subdivisions + 1) * (subdivisions + 1);
    int num_faces = subdivisions * subdivisions * 2;

    mesh->num_vertices = num_vertices;
    mesh->num_faces = num_faces;
    mesh->vertices = malloc(sizeof(Vector3) * num_vertices);
    mesh->faces = malloc(sizeof(Face) * num_faces);

    int index = 0;
    for (int lat = 0; lat <= subdivisions; lat++) {
        float theta = lat * M_PI / subdivisions;
        float sin_theta = sinf(theta);
        float cos_theta = cosf(theta);

        for (int lon = 0; lon <= subdivisions; lon++) {
            float phi = lon * 2 * M_PI / subdivisions;
            float sin_phi = sinf(phi);
            float cos_phi = cosf(phi);

            float x = sin_theta * cos_phi;
            float y = cos_theta;
            float z = sin_theta * sin_phi;

            mesh->vertices[index++] = (Vector3){x, y, z};
        }
    }

    index = 0;
    for (int lat = 0; lat < subdivisions; lat++) {
        for (int lon = 0; lon < subdivisions; lon++) {
            int current = lat * (subdivisions + 1) + lon;
            int next = current + subdivisions + 1;

            mesh->faces[index++] = (Face){current, next, current + 1};
            mesh->faces[index++] = (Face){current + 1, next, next + 1};
        }
    }
}

void draw_laser(Vector3 start, Vector3 end) {
    int x1, y1, x2, y2;
    project_point(start, &x1, &y1);
    project_point(end, &x2, &y2);

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        engine_put_pixel(x1, y1, LASER_COLOR);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
}

void init_demo_scene(void) {
    create_sphere(&sphere, 16);
    create_spaceship(&spaceship1);
    create_spaceship(&spaceship2);
    create_spaceship(&spaceship3);
    create_enterprise(&enterprise);  // Add this line


    center_mesh(&sphere);
    center_mesh(&spaceship1);
    center_mesh(&spaceship2);
    center_mesh(&spaceship3);
    center_mesh(&enterprise);  // Add this line

    scale_mesh(&spaceship1, 0.2f);
    scale_mesh(&spaceship2, 0.2f);
    scale_mesh(&spaceship3, 0.2f);
    scale_mesh(&enterprise, 0.3f);  // Add this line, slightly larger


    srand(time(NULL));  // Initialize random seed
}

void render_demo_scene(float rotation_x, float rotation_y, float rotation_z, float zoom_factor) {
    (void)rotation_y;  // Silence unused parameter warning

    sphere_rotation += 0.004f;

    // Render the rotating sphere
    render_mesh(&sphere, rotation_x, sphere_rotation, rotation_z, zoom_factor * 2.0f, 0);

    demo_time += 0.001f;
    float orbit_radius = 8.5f;

    // Equatorial orbit (spaceship1)
    float x1 = cosf(demo_time) * orbit_radius;
    float y1 = 0;
    float z1 = sinf(demo_time) * orbit_radius;

    // Polar orbit (spaceship2)
    float x2 = 0;
    float y2 = cosf(demo_time) * orbit_radius;
    float z2 = sinf(demo_time) * orbit_radius;

    // Stationary position (spaceship3)
    float x3 = orbit_radius * 0.5f;
    float y3 = orbit_radius * 0.5f;
    float z3 = orbit_radius * 0.5f;

    // Enterprise orbit (opposite to spaceship1)
    float x4 = cosf(demo_time + M_PI) * orbit_radius;
    float y4 = 0;
    float z4 = sinf(demo_time + M_PI) * orbit_radius;

    // Render spaceship1 (equatorial orbit)
    Mesh temp_ship1 = copy_mesh(&spaceship1);
    //Mesh temp_ship1 = copy_mesh(&cow_mesh);
    translate_mesh(&temp_ship1, x1, y1, z1);
    render_mesh(&temp_ship1, 0, -demo_time, M_PI/2, zoom_factor * 0.3f, 0);

    // Render spaceship2 (polar orbit)
    //Mesh temp_ship2 = copy_mesh(&spaceship2);
    Mesh temp_ship2 = copy_mesh(&homer_mesh);
    translate_mesh(&temp_ship2, x2, y2, z2);
    render_mesh(&temp_ship2, demo_time, M_PI/2, 0, zoom_factor * 0.3f, 0);

    // Render spaceship3 (stationary)
    Mesh temp_ship3 = copy_mesh(&cow_mesh);
    translate_mesh(&temp_ship3, x3, y3, z3);
    render_mesh(&temp_ship3, 0, demo_time * 0.5f, 0, zoom_factor * 0.3f, 0);

    // Render Enterprise (equatorial orbit opposite to spaceship1)
    //Mesh temp_enterprise = copy_mesh(&enterprise);
    //Mesh temp_enterprise = copy_mesh(&cow_mesh);
    Mesh temp_enterprise = copy_mesh(&spaceship3);
    translate_mesh(&temp_enterprise, x4, y4, z4);
    render_mesh(&temp_enterprise, 0, -demo_time, M_PI/2, zoom_factor * 0.3f, 0);

    free_mesh(&temp_ship1);
    free_mesh(&temp_ship2);
    free_mesh(&temp_ship3);
    free_mesh(&temp_enterprise);
}


void cleanup_demo_scene(void) {
    free_mesh(&sphere);
    free_mesh(&spaceship1);
    free_mesh(&spaceship2);
    free_mesh(&spaceship3);
    free_mesh(&enterprise);  // Add this line
}
