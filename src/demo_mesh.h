#ifndef DEMO_MESH_H
#define DEMO_MESH_H

#include "3d_engine.h"

Vector3 exported_vertices[] = {
    {-0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f, -0.5f},
    { 0.5f,  0.5f, -0.5f},
    {-0.5f,  0.5f, -0.5f},
    {-0.5f, -0.5f,  0.5f},
    { 0.5f, -0.5f,  0.5f},
    { 0.5f,  0.5f,  0.5f},
    {-0.5f,  0.5f,  0.5f}
};

Face exported_faces[] = {
    {0, 1, 2}, {0, 2, 3},
    {1, 5, 6}, {1, 6, 2},
    {5, 4, 7}, {5, 7, 6},
    {4, 0, 3}, {4, 3, 7},
    {3, 2, 6}, {3, 6, 7},
    {4, 5, 1}, {4, 1, 0}
};

Mesh exported_mesh = {
    .vertices = exported_vertices,
    .faces = exported_faces,
    .num_vertices = sizeof(exported_vertices) / sizeof(exported_vertices[0]),
    .num_faces = sizeof(exported_faces) / sizeof(exported_faces[0])
};

#endif
