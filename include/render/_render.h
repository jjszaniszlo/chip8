#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <linmath.h>

typedef struct {
  uint32_t vao_quad;
  uint32_t vbo_quad;
  uint32_t ebo_quad;
  uint32_t shader_default;
  uint32_t texture_color;
  mat4x4 projection;
} internal_render_state;

GLFWwindow *render_init_window(uint32_t width, uint32_t height);
void render_init_quad(uint32_t *vao, uint32_t *vbo, uint32_t *ebo);
void render_init_color_texture(uint32_t *texture);
void render_init_shaders(internal_render_state*state);
uint32_t render_shader_create(const char *path_vert, const char *path_frag);
