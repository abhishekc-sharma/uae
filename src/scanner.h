#ifndef UAE_SCANNER_H
#define UAE_SCANNER_H

#include <stdio.h>
#include "input_buffer.h"
#include "token.h"


typedef struct {
  input_buffer_t *input_buffer;
  long long line;
  long long col;
} scanner_t;

scanner_t *scanner_new(FILE *);
void scanner_delete(scanner_t **);
token_t *scanner_token(scanner_t *);
void scanner_flush_line(scanner_t *);
#endif
