#include "scanner.h"
#include "token.h"
#include "input_buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

scanner_t *scanner_new(FILE *fp) {
  scanner_t *scanner = (scanner_t *) malloc(sizeof(scanner_t));

  scanner->input_buffer = input_buffer_new(fp);
  scanner->line = 0;
  scanner->col = 1;

  return scanner;
}

token_t *scanner_token(scanner_t *scanner) {
  if(feof(scanner->input_buffer->fp)) {
    return token_new(T_EOF, scanner->line, scanner->col);
  }
  char c;
  while((c = input_buffer_getchar(scanner->input_buffer)) != EOF) {
    scanner->col++;
    switch(c) {
      case '0':
        return token_new(T_ZERO, scanner->line, scanner->col);
        break;

      case '(':
        return token_new(T_LEFT_PAREN, scanner->line, scanner->col);
        break;

      case ')':
        return token_new(T_RIGHT_PAREN, scanner->line, scanner->col);
        break;

      case '\n':
        scanner->line++;
        scanner->col = 0;
      case ' ':
      case '\t':
        break;

      default:
        input_buffer_ungetchar(scanner->input_buffer, c);
        scanner->col--;
        char *temp_buf = (char *) malloc((MAX_TOKEN_LENGTH + 1) * sizeof(char));
        char *ptr = temp_buf;
        while((c = input_buffer_getchar(scanner->input_buffer)) != EOF && (c >= 'a' && c <= 'z') && (ptr - temp_buf <= MAX_TOKEN_LENGTH)) {
          *ptr = c;
          ptr++;
          scanner->col++;
        }

        if(ptr - temp_buf > MAX_TOKEN_LENGTH) {
          return NULL;
        }

        if(!(c >= 'a' && c <= 'z')) {
          input_buffer_ungetchar(scanner->input_buffer, c);
        }

        *ptr = '\0';

        token_t *token;
        size_t l = strlen(temp_buf);
        if(strcmp(temp_buf, "true") == 0) {
          token = token_new(T_TRUE, scanner->line, scanner->col - l);
        } else if(strcmp(temp_buf, "false") == 0) {
          token = token_new(T_FALSE, scanner->line, scanner->col - l);
        } else if(strcmp(temp_buf, "succ") == 0) {
          token = token_new(T_SUCC, scanner->line, scanner->col - l);
        } else if(strcmp(temp_buf, "pred") == 0) {
          token = token_new(T_PRED, scanner->line, scanner->col - l);
        } else if(strcmp(temp_buf, "iszero") == 0) {
          token = token_new(T_ISZERO, scanner->line, scanner->col - l);
        } else if(strcmp(temp_buf, "if") == 0) {
          token = token_new(T_IF, scanner->line, scanner->col - l);
        } else if(strcmp(temp_buf, "then") == 0) {
          token = token_new(T_THEN, scanner->line, scanner->col - l);
        } else if(strcmp(temp_buf, "else") == 0) {
          token = token_new(T_ELSE, scanner->line, scanner->col - l);
        } else {
          return NULL;
        }

        free(temp_buf);
        return token;
        break;
    }
  }

  if(feof(scanner->input_buffer->fp)) {
    return token_new(T_EOF, scanner->line, scanner->col);
  }
}

void scanner_delete(scanner_t **scanner_ptr) {
  input_buffer_delete(&(*scanner_ptr)->input_buffer);
  free(*scanner_ptr);
  *scanner_ptr = NULL;
}

void scanner_flush_line(scanner_t *scanner) {
  while(input_buffer_getchar(scanner->input_buffer) != '\n') ;
}
