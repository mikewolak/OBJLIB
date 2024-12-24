#include <SDL2/SDL.h>
#include "3d_engine.h"
#include "obj_loader.h"
#include "sdl_wrapper.h"
#include "obj_exporter.h"
#include "demo_mesh.h"
#include "demo_scene.h"
#include <stdio.h>
#include <string.h>

#define INITIAL_WINDOW_WIDTH 800
#define INITIAL_WINDOW_HEIGHT 600

int main(int argc, char *argv[]) {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;

    printf("Initializing SDL...\n");
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL initialization failed: %s", SDL_GetError());
        return 1;
    }

    printf("Creating window...\n");
    window = SDL_CreateWindow("3D OBJ Viewer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_Log("Window creation failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    printf("Creating renderer...\n");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Renderer creation failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    printf("Creating texture...\n");
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);
    if (!texture) {
        SDL_Log("Texture creation failed: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    printf("Initializing SDL wrapper...\n");
    init_sdl_wrapper(renderer, texture, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);

    printf("Initializing 3D engine...\n");
    init_3d_engine(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);
    set_put_pixel_callback(put_pixel);

    int run_demo = 0;
    if (argc > 1 && strcmp(argv[1], "run_demo") == 0) {
        run_demo = 1;
        init_demo_scene();
    }

    Mesh mesh;
    if (!run_demo) {
        if (argc > 1) {
            printf("Loading OBJ file: %s\n", argv[1]);
            if (!load_obj(&mesh, argv[1])) {
                SDL_Log("Failed to load OBJ file: %s", argv[1]);
                mesh = exported_mesh;
            } else {
                // Export the loaded mesh to a header file
                export_mesh_to_header(&mesh, "exported_mesh.h");
            }
        } else {
            printf("Using demo mesh\n");
            mesh = exported_mesh;
        }

        center_mesh(&mesh);
    }

    float zoom_factor = run_demo ? 0.2f : fit_mesh_to_screen(&mesh, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);
    printf("Initial zoom factor: %f\n", zoom_factor);
    printf("Total vertices: %d, Total triangles: %d\n", mesh.num_vertices, mesh.num_faces);

    SDL_Event event;
    int quit = 0;
    float rotation_x = 0, rotation_y = 0, rotation_z = 0;
    float rotation_speed_x = 0, rotation_speed_y = 0;
    int prev_mouse_x = 0, prev_mouse_y = 0;
    int window_width = INITIAL_WINDOW_WIDTH, window_height = INITIAL_WINDOW_HEIGHT;
    int thick_lines = 1;

    printf("Entering main loop...\n");
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = 1;
                    break;
                case SDL_MOUSEMOTION:
                    if (event.motion.state & SDL_BUTTON_LMASK) {
                        rotation_speed_y = (event.motion.x - prev_mouse_x) * 0.001f;
                        rotation_speed_x = (event.motion.y - prev_mouse_y) * 0.001f;
                    }
                    prev_mouse_x = event.motion.x;
                    prev_mouse_y = event.motion.y;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            quit = 1;
                            break;
                        case SDLK_SPACE:
                            rotation_x = rotation_y = rotation_z = 0;
                            rotation_speed_x = rotation_speed_y = 0;
                            break;
                        case SDLK_UP:
                            rotation_x += 0.1f;
                            break;
                        case SDLK_DOWN:
                            rotation_x -= 0.1f;
                            break;
                        case SDLK_LEFT:
                            rotation_y -= 0.1f;
                            break;
                        case SDLK_RIGHT:
                            rotation_y += 0.1f;
                            break;
                        case SDLK_z:
                            if (event.key.keysym.mod & KMOD_SHIFT) {
                                zoom_factor /= 1.1f;
                                printf("Zoom factor: %f\n", zoom_factor);
                            } else {
                                zoom_factor *= 1.1f;
                            }
                            break;
                        case SDLK_t:
                            thick_lines = !thick_lines;
                            printf("Thick lines %s\n", thick_lines ? "enabled" : "disabled");
                            break;
                    }
                    break;
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        window_width = event.window.data1;
                        window_height = event.window.data2;
                        resize_sdl_wrapper(window_width, window_height);
                        resize_3d_engine(window_width, window_height);
                        SDL_DestroyTexture(texture);
                        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                                    window_width, window_height);
                        init_sdl_wrapper(renderer, texture, window_width, window_height);
                    }
                    break;
            }
        }

        rotation_x += rotation_speed_x;
        rotation_y += rotation_speed_y;

        clear_screen();
        if (run_demo) {
            render_demo_scene(rotation_x, rotation_y, rotation_z, zoom_factor);
        } else {
            render_mesh(&mesh, rotation_x, rotation_y, rotation_z, zoom_factor, thick_lines);
        }
        update_screen();

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    printf("Exiting main loop...\n");

    if (run_demo) {
        cleanup_demo_scene();
    } else {
        free_mesh(&mesh);
    }

    printf("Cleaning up SDL wrapper...\n");
    cleanup_sdl_wrapper();

    printf("Destroying SDL objects...\n");
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    printf("Quitting SDL...\n");
    SDL_Quit();

    printf("Program finished.\n");
    return 0;
}
