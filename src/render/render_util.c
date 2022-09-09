#include <glad/glad.h>
#include <stdio.h>

#include "io.h"
#include "render/_render.h"

uint32_t render_shader_create(const char *path_vert, const char *path_frag) {
  int sucess;
  char log[512];

  file file_vertex = io_read(path_vert);
  if (!file_vertex.is_valid) {
    printf("error: file couldn't be opened!");
    exit(-1);
  }

  uint32_t shader_vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(shader_vertex, 1, (const char *const *)&file_vertex, NULL);
  glCompileShader(shader_vertex);
  glGetShaderiv(shader_vertex, GL_COMPILE_STATUS, &sucess);
  if (!sucess) {
    glGetShaderInfoLog(shader_vertex, 512, NULL, log);
    printf("error: compiling vertex shader failed. %s", log);
    exit(-1);
  }

  file file_fragment = io_read(path_frag);
  if (!file_fragment.is_valid) {
    printf("error: reading vertex shader failed. %s", path_frag);
    exit(-1);
  }

  uint32_t shader_fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(shader_fragment, 1, (const char *const *)&file_fragment, NULL);
  glCompileShader(shader_fragment);
  glGetShaderiv(shader_fragment, GL_COMPILE_STATUS, &sucess); 
  if (!sucess) {
    glGetShaderInfoLog(shader_fragment, 512, NULL, log);
    printf("error: compiling fragment shader failed. %s", path_frag);
    exit(-1);
  }

  uint32_t shader = glCreateProgram();
  glAttachShader(shader, shader_vertex);
  glAttachShader(shader, shader_fragment);
  glLinkProgram(shader);
  glGetProgramiv(shader, GL_LINK_STATUS, &sucess);
  if (!sucess) {
    glGetShaderInfoLog(shader, 512, NULL, log);
    printf("error: compiling linking shader failed. %s", path_frag);
    exit(-1);
  }
  
  free(file_vertex.data);
  free(file_fragment.data);
  
  return shader;
}


