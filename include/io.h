#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
  char *data;
  uint32_t len;
  bool is_valid;
} file;

file io_read(const char *path);
int32_t io_write(void *buffer, const size_t size, const char *path);
