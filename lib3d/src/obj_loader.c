#include "obj_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256

int load_obj(Mesh *mesh, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return 0;
    }

    Vector3 *vertices = NULL;
    Face *faces = NULL;
    int num_vertices = 0;
    int num_faces = 0;
    int max_vertices = 1000;
    int max_faces = 1000;

    vertices = malloc(sizeof(Vector3) * max_vertices);
    faces = malloc(sizeof(Face) * max_faces);

    char line[MAX_LINE];
    while (fgets(line, MAX_LINE, file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            if (num_vertices >= max_vertices) {
                max_vertices *= 2;
                vertices = realloc(vertices, sizeof(Vector3) * max_vertices);
            }
            sscanf(line, "v %f %f %f", &vertices[num_vertices].x, &vertices[num_vertices].y, &vertices[num_vertices].z);
            num_vertices++;
        } else if (line[0] == 'f' && line[1] == ' ') {
            if (num_faces >= max_faces) {
                max_faces *= 2;
                faces = realloc(faces, sizeof(Face) * max_faces);
            }
            sscanf(line, "f %d %d %d", &faces[num_faces].v1, &faces[num_faces].v2, &faces[num_faces].v3);
            faces[num_faces].v1--;
            faces[num_faces].v2--;
            faces[num_faces].v3--;
            num_faces++;
        }
    }

    fclose(file);

    mesh->vertices = vertices;
    mesh->faces = faces;
    mesh->num_vertices = num_vertices;
    mesh->num_faces = num_faces;

    return 1;
}
