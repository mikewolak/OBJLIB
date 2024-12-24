#ifndef DEMO_H
#define DEMO_H

#include "3d_engine.h"

Vector3 demo_vertices[] = {
    {-0.5f, -0.5f,  0.5f},
    { 0.5f, -0.5f,  0.5f},
    { 0.5f,  0.5f,  0.5f},
    {-0.5f,  0.5f,  0.5f},
    {-0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f, -0.5f},
    { 0.5f,  0.5f, -0.5f},
    {-0.5f,  0.5f, -0.5f}
};

Face demo_faces[] = {
    {0, 3, 2}, {0, 2, 1},
    {1, 2, 6}, {1, 6, 5},
    {5, 6, 7}, {5, 7, 4},
    {4, 7, 3}, {4, 3, 0},
    {3, 7, 6}, {3, 6, 2},
    {4, 0, 1}, {4, 1, 5}
};

Mesh demo_mesh = {
    .vertices = demo_vertices,
    .faces = demo_faces,
    .num_vertices = 8,
    .num_faces = 12
};

#endif // DEMO_H
