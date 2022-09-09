#include "app.h"

#include "render/render.h"
#include "cpu.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

app_state g_state = {0};

void app_init(void) {
  cpu_init(&g_state.cpu, "roms/invaders.rom");
  render_init();
}

void app_run(void) {
  double chip_delay = 1.0f/60.0f;
  double dt, current_time, past_time;
  past_time = glfwGetTime();

  /* glfwSetKeyCallback(g_state.renderer.window, app_key_callback); */
  glfwSetInputMode(g_state.renderer.window, GLFW_STICKY_KEYS, 1);

  do {
    current_time = glfwGetTime();
    dt = current_time - past_time;

    /* if (dt > chip_delay) { */
    /*   past_time = current_time; */
    cpu_emulate(&g_state.cpu);

    render_begin();

    cpu_render(&g_state.cpu);
    
    render_end();

    glfwPollEvents();
  } while (glfwGetKey(g_state.renderer.window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(g_state.renderer.window) == 0);

  /* cpu_print_state(&g_state.cpu); */
}

void app_destroy(void) {
  glfwDestroyWindow(g_state.renderer.window);
  glfwTerminate();
}

void app_key_callback(GLFWwindow *w, int key, int scancode, int action, int mods) {
  cpu_keyhandle(&g_state.cpu, key, scancode, action, mods);
}
