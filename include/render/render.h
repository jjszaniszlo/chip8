#pragma once

#include "cpu.h"
#include <stdbool.h>
#include <SDL2/SDL.h>

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  uint32_t w;
  uint32_t h;
} render_state;

bool render_init(render_state *state);
void render_destroy();
