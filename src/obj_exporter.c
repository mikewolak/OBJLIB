#include "obj_exporter.h"
#include <stdio.h>

void export_mesh_to_header(const Mesh *mesh, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Failed to open file for writing: %s\n", filename);
        return;
    }

    fprintf(file, "#ifndef EXPORTED_MESH_H\n");
    fprintf(file, "#define EXPORTED_MESH_H\n\n");
    fprintf(file, "#include \"3d_engine.h\"\n\n");

    fprintf(file, "Vector3 exported_vertices[] = {\n");
    for (int i = 0; i < mesh->num_vertices; i++) {
        fprintf(file, "    {%ff, %ff, %ff},\n", mesh->vertices[i].x, mesh->vertices[i].y, mesh->vertices[i].z);
    }
    fprintf(file, "};\n\n");

    fprintf(file, "Face exported_faces[] = {\n");
    for (int i = 0; i < mesh->num_faces; i++) {
        fprintf(file, "    {%d, %d, %d},\n", mesh->faces[i].v1, mesh->faces[i].v2, mesh->faces[i].v3);
    }
    fprintf(file, "};\n\n");

    fprintf(file, "Mesh exported_mesh = {\n");
    fprintf(file, "    .vertices = exported_vertices,\n");
    fprintf(file, "    .faces = exported_faces,\n");
    fprintf(file, "    .num_vertices = %d,\n", mesh->num_vertices);
    fprintf(file, "    .num_faces = %d\n", mesh->num_faces);
    fprintf(file, "};\n\n");

    fprintf(file, "#endif // EXPORTED_MESH_H\n");

    fclose(file);
    printf("Mesh exported to %s\n", filename);
}
