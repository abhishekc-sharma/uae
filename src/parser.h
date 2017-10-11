#ifndef UAE_PARSER_H
#define UAE_PARSER_H

#include <stdio.h>
#include "ast.h"
#include "scanner.h"
#include "token.h"

typedef struct {
  scanner_t *scanner;
  token_t *saved;
} parser_t;

#define MAX_PARSER_ERROR_MSG_SIZE 100
extern char parser_error_message[MAX_PARSER_ERROR_MSG_SIZE];

parser_t *parser_new(FILE *);
void parser_delete(parser_t **);
astnode_t *parse_term(parser_t *);

#endif
