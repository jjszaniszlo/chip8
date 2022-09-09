#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "app.h"
#include "render/_render.h"
#include "render/render.h"

GLFWwindow *render_init_window(uint32_t width, uint32_t height) {
  if (!glfwInit()) {
    printf("error: failed to init glfw!");
    exit(-1);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create window
  GLFWwindow *window;
  window = glfwCreateWindow(width, height, "chip8", NULL, NULL);
  if (window == NULL) {
    printf("error: failed to open glfw window!");
    exit(-1);
  }

  // Create context
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("error: failed to load opengl context!");
    exit(-1);
  }

  return window;
}

void render_init_shaders(internal_render_state *state) {
  state->shader_default = render_shader_create("./shaders/default.vert", "./shaders/default.frag");

  mat4x4_ortho(state->projection, 0, g_state.renderer.width, 0, g_state.renderer.height, -2, 2);

  glUseProgram(state->shader_default);
  glUniformMatrix4fv(
    glGetUniformLocation(state->shader_default, "projection"),
    1,
    GL_FALSE,
    &state->projection[0][0]
  );
}

void render_init_color_texture(uint32_t *texture) {
  glGenTextures(1, texture);
  glBindTexture(GL_TEXTURE_2D, *texture);

  uint8_t solid_white[4] = {255, 255, 255, 255};
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, solid_white);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void render_init_quad(uint32_t *vao, uint32_t *vbo, uint32_t *ebo) {
  //  x,    y,  z, u, v
  float vertices[] = {
    0.5,  0.5,  0, 0, 0,
    0.5, -0.5,  0, 0, 1,
   -0.5, -0.5,  0, 1, 1,
   -0.5,  0.5,  0, 1, 0,
  };

  uint32_t indicies[] = {
    0, 1, 3,
    1, 2, 3,
  };

  glGenVertexArrays(1, vao);
  glGenBuffers(1, vbo);
  glGenBuffers(1, ebo);

  glBindVertexArray(*vao);

  glBindBuffer(GL_ARRAY_BUFFER, *vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

  // xyz
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
  glEnableVertexAttribArray(0);

  // uv
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
  glad_glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}
