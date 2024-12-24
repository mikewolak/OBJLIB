#ifndef SDL_WRAPPER_H
#define SDL_WRAPPER_H

#include <SDL2/SDL.h>

void init_sdl_wrapper(SDL_Renderer *renderer, SDL_Texture *texture, int width, int height);
void resize_sdl_wrapper(int width, int height);
void put_pixel(int x, int y, uint32_t color);
void update_screen(void);
void clear_screen(void);
void cleanup_sdl_wrapper(void);

#endif
