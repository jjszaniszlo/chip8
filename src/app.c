#include "app.h"

#include "cpu.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

app_state g_state = {0};

void app_init(void) {
  cpu_init(&g_state.cpu, "roms/tester.ch8");
  render_init();
}

void app_run(void) {
  double chip_delay = 1.0f/540.0f;
  double dt, current_time, past_time;
  past_time = glfwGetTime();
  double dt2, current_time2, past_time2;
  past_time2 = glfwGetTime();

  /* glfwSetKeyCallback(g_state.renderer.window, app_key_callback); */
  glfwSetInputMode(g_state.renderer.window, GLFW_STICKY_KEYS, 1);

  do {
    current_time = glfwGetTime();
    dt = current_time - past_time;

    if (dt > chip_delay) {
      past_time = current_time;
      cpu_emulate(&g_state.cpu);
    }

    current_time2 = glfwGetTime();
    dt2 = current_time2 - past_time2;

    if (dt2 > (1.0f / 60.0f)) {
      past_time2 = current_time2;
      if (g_state.cpu.delay > 0)
        g_state.cpu.delay--;

      if (g_state.cpu.sound > 0)
        g_state.cpu.sound--;
    }

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
