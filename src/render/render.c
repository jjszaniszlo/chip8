#include "render/render.h"
#include "SDL2/SDL_video.h"

bool render_init(render_state *state) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("error: sdl could not be initialized!\n");
    return false;
  }

  state->w = 64 * 16;
  state->h = 32 * 16;
  state->window = SDL_CreateWindow("chip8", SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED, state->w, state->h,
                                   SDL_WINDOW_SHOWN);
  if (state->window == NULL) {
    printf("error: sdl_window could not be created!\n");
    return false;
  }

  state->renderer =
      SDL_CreateRenderer(state->window, -1, SDL_RENDERER_ACCELERATED);

  if (state->renderer == NULL) {
    printf("error: sdl_renderer could not be created!\n");
    return false;
  }
  return true;
}

void render_destroy(render_state *state) {
  SDL_DestroyWindow(state->window);
  SDL_Quit();
}
