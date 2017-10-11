#include "input_buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

input_buffer_t *input_buffer_new(FILE *fp) {
  input_buffer_t *input_buffer = malloc(sizeof(input_buffer_t));
  input_buffer->fp = fp;
  return input_buffer;
}

void *input_buffer_delete(input_buffer_t **input_buffer_ptr) {
  free(*input_buffer_ptr);
  *input_buffer_ptr = NULL;
}

char input_buffer_getchar(input_buffer_t *input_buffer) {
  return fgetc(input_buffer->fp);
}

char input_buffer_ungetchar(input_buffer_t *input_buffer, char c) {
  return ungetc(c, input_buffer->fp);
}
