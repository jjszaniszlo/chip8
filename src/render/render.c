#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "app.h"
#include "linmath.h"
#include "render/render.h"
#include "render/_render.h"

static internal_render_state state = {0};

void render_init(void) {
  g_state.renderer.width = 1024;
  g_state.renderer.height = 512;
  g_state.renderer.window = render_init_window(g_state.renderer.width, g_state.renderer.height);

  render_init_quad(&state.vao_quad, &state.vbo_quad, &state.ebo_quad);
  render_init_shaders(&state);
  render_init_color_texture(&state.texture_color);
}

void render_begin(void) {
  // clear screen
  glClearColor(0.01, 0.2, 0.2, 1);
  glClear(GL_COLOR_BUFFER_BIT);
}

void render_end(void) {
  // swap buffers to display the new buffer, then to use the previouslly used one.
  glfwSwapBuffers(g_state.renderer.window);
}

void render_quad(vec2 pos, vec2 size, vec4 color) {
  // use the program compiled and assigned to the state.shader_default ID.
  glUseProgram(state.shader_default);

  // create a basic 4x4 translation matrix.
  mat4x4 model;
  mat4x4_identity(model);

  // scale and translate matrix to positiona and scale the quad.
  mat4x4_translate(model, pos[0], pos[1], 0);
  mat4x4_scale_aniso(model, model, size[0], size[1], 1);

  // apply the transformation and color to the input of the shader program.
  glUniformMatrix4fv(glGetUniformLocation(state.shader_default, "model"), 1, GL_FALSE, &model[0][0]);
  glUniform4fv(glad_glGetUniformLocation(state.shader_default, "color"), 1, color);

  // bind the vertex arrays in vao_quad.
  glBindVertexArray(state.vao_quad);

  // bind the color texture and draw the element.
  glBindTexture(GL_TEXTURE_2D, state.texture_color);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

  // unbind the vertex arrays.
  glBindVertexArray(0);
}
