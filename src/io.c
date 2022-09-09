#include "io.h"

#include <errno.h>
#include <stdio.h>

#define IO_READ_CHUNK_SIZE 2097152
#define IO_READ_ERROR_GENERAL "Error reading file: %s. errno: %d\n"
#define IO_READ_ERROR_MEMORY "Not enough free memory to read file: %s\n"

file io_read(const char *path) {
  file f = {.is_valid = false};

  FILE *fp = fopen(path, "rb");
  if (!fp || ferror(fp)) {
    printf("error: could not open %s", path);
    exit(-1);
  }

  char *data = NULL;
  char *tmp;
  size_t used = 0;
  size_t size = 0;
  size_t n;

  while (true) {
    if (used + IO_READ_CHUNK_SIZE + 1 > size) {
      size = used + IO_READ_CHUNK_SIZE + 1;

      if (size <= used) {
        free(data);
        printf("input file too large: %s\n", path);
        return f;
      }

      tmp = realloc(data, size);
      if (!tmp) {
        free(data);
        printf(IO_READ_ERROR_MEMORY, path);
        return f;
      }
      data = tmp;
    }
    n = fread(data + used, 1, IO_READ_CHUNK_SIZE, fp);
    if (n == 0)
      break;

    used += n;
  }

  if (ferror(fp)) {
    free(data);
    printf(IO_READ_ERROR_GENERAL, path, errno);
    return f;
  }

  tmp = realloc(data, used + 1);

  if (!tmp) {
    free(data);
    printf(IO_READ_ERROR_MEMORY, path, errno);
    return f;
  }

  data = tmp;
  data[used] = 0;

  f.data = data;
  f.len = used;
  f.is_valid = true;

  return f;
}

int32_t io_write(void *buffer, const size_t size, const char *path) {
  FILE *fp = fopen(path, "wb");

  if (!fp || ferror(fp)) {
    printf("cannot write to file: %s\n", path);
    return 1;
  }

  size_t chunks_written = fwrite(buffer, size, 1, fp);
  fclose(fp);

  if (chunks_written != 1) {
    printf("Write Error. Expected 1 chunk, got %zu\n", chunks_written);
    return 1;
  }

  return 0;
}
