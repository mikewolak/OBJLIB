#include "sdl_wrapper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static SDL_Renderer *renderer;
static SDL_Texture *texture;
static uint32_t *pixels;
static int screen_width, screen_height;

void init_sdl_wrapper(SDL_Renderer *r, SDL_Texture *t, int width, int height) {
    renderer = r;
    texture = t;
    screen_width = width;
    screen_height = height;
    
    pixels = (uint32_t*)calloc(width * height, sizeof(uint32_t));
    if (!pixels) {
        printf("Failed to allocate memory for pixels\n");
        exit(1);
    }
    
    printf("SDL wrapper initialized. Width: %d, Height: %d\n", width, height);
}

void resize_sdl_wrapper(int width, int height) {
    screen_width = width;
    screen_height = height;
    
    free(pixels);
    pixels = (uint32_t*)calloc(width * height, sizeof(uint32_t));
    if (!pixels) {
        printf("Failed to reallocate memory for pixels\n");
        exit(1);
    }
    
    printf("SDL wrapper resized. New Width: %d, New Height: %d\n", width, height);
}

void put_pixel(int x, int y, uint32_t color) {
    if (x >= 0 && x < screen_width && y >= 0 && y < screen_height) {
        pixels[y * screen_width + x] = color;
    }
}

void update_screen(void) {
    void *texture_pixels;
    int texture_pitch;

    if (SDL_LockTexture(texture, NULL, &texture_pixels, &texture_pitch) != 0) {
        printf("Failed to lock texture: %s\n", SDL_GetError());
        return;
    }

    memcpy(texture_pixels, pixels, screen_height * screen_width * sizeof(uint32_t));

    SDL_UnlockTexture(texture);
}

void clear_screen(void) {
    memset(pixels, 0, screen_width * screen_height * sizeof(uint32_t));
}

void cleanup_sdl_wrapper(void) {
    free(pixels);
}
