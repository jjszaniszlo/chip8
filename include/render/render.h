#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <linmath.h>

typedef struct {
  GLFWwindow *window;
  float width;
  float height;
} render_state;

void render_init(void);
void render_begin(void);
void render_end(void);
void render_quad(vec2 pos, vec2 size, vec4 color);
