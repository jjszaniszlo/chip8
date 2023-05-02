#include "app.h"
#include "SDL2/SDL_render.h"
#include "render/render.h"

#include "cpu.h"

app_state g_state = {0};

void app_init(void) {
  cpu_init(&g_state.cpu, "roms/tester.ch8");
  render_init(&g_state.render);
}

typedef struct {
  double ctime, ptime, dt;
} deltatime;

void init_dt(deltatime *dt) {
  dt->ptime = SDL_GetTicks();
}

void calc_dt(deltatime *dt) {
  dt->ctime = SDL_GetTicks();
  dt->dt = dt->ctime - dt->ptime;
}

bool is_dt_over(deltatime *dt, long value) {
  return dt->dt > value;
}

void app_run(void) {
  SDL_Event e;
  bool should_quit = false;

  // two timers for 60hz and 540hz.
  deltatime dt1 = {0};
  init_dt(&dt1);
  deltatime dt2 = {0};
  init_dt(&dt2);

  do {
    calc_dt(&dt1);
    calc_dt(&dt2);

    // cycle cpu every 540hz.
    if (is_dt_over(&dt1, (long)(1.0l / 540.0l))) {
      dt1.ptime = SDL_GetTicks();
      cpu_emulate(&g_state.cpu);
    }

    // decrement every 60hz
    if (is_dt_over(&dt2, (long)(1.0l / 60.0l))) {
      dt2.ptime = SDL_GetTicks();
      if (g_state.cpu.delay > 0) g_state.cpu.delay--;
      if (g_state.cpu.sound > 0) g_state.cpu.sound--;
    }

    // handle events
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_QUIT: should_quit = true; break;
      case SDL_KEYDOWN:
        cpu_keyhandle(&g_state.cpu, e.key.keysym.scancode, true);
        break;
      case SDL_KEYUP:
        cpu_keyhandle(&g_state.cpu, e.key.keysym.scancode, false);
        break;
      }
    }

    // clear screen.
    SDL_SetRenderDrawColor(g_state.render.renderer, 0, 0, 0, 255);
    SDL_RenderClear(g_state.render.renderer);

    // draw cpu scren buffer.
    cpu_render(&g_state.cpu);

    // swap buffers and present.
    SDL_RenderPresent(g_state.render.renderer);
  } while (!should_quit);
}

void app_destroy(void) {
  render_destroy(&g_state.render);
}
