#ifndef UAE_INPUT_BUFFER_H
#define UAE_INPUT_BUFFER_H

#include <stdio.h>

typedef struct {
  FILE *fp;
} input_buffer_t;

input_buffer_t *input_buffer_new(FILE *);
void *input_buffer_delete(input_buffer_t **);
char input_buffer_getchar(input_buffer_t *);
char input_buffer_ungetchar(input_buffer_t *, char);

#endif
