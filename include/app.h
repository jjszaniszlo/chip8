#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "render/render.h"
#include "cpu.h"

typedef struct {
  render_state renderer;
  cpu_state cpu;
} app_state;

extern app_state g_state;

void app_init(void);
void app_run(void);
void app_destroy(void);

void app_key_callback(GLFWwindow *w, int key, int scancode, int action, int mods);
