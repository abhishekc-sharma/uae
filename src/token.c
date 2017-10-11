#include "token.h"
#include <stdlib.h>
#include <stdio.h>

int token_is_value(token_t *token) {
  switch(token->type) {
      case T_TRUE:
      case T_FALSE:
      case T_ZERO:
        return 1;
      default:
        return 0;
  }
}

int token_is_keyword(token_t *token) {
  switch(token->type) {
      case T_SUCC:
      case T_PRED:
      case T_ISZERO:
      case T_IF:
      case T_THEN:
      case T_ELSE:
        return 1;
      default:
        return 0;
  }
}

int token_is_seperator(token_t *token) {
  switch(token->type) {
      case T_LEFT_PAREN:
      case T_RIGHT_PAREN:
        return 1;
      default:
        return 0;
  }
}

int token_is(token_t *token, token_type_t want) {
  return token != NULL && (token->type == want);
}

token_t *token_new(token_type_t type, long long line, long long col) {
  token_t *token = (token_t *) malloc(sizeof(token_t));
  token->type = type;
  token->line = line;
  token->col = col;

  return token;
}

void token_delete(token_t **token_ptr) {
  free(*token_ptr);
  *token_ptr = NULL;
}



void print_token(token_t *token) {
    printf("Type: ");
  switch(token->type) {
    case T_TRUE:
      printf("true");
      break;
    case T_FALSE:
      printf("false");
      break;
    case T_ZERO:
      printf("0");
      break;
    case T_SUCC:
      printf("succ");
      break;
    case T_PRED:
      printf("pred");
      break;
    case T_ISZERO:
      printf("iszero");
      break;
    case T_IF:
      printf("if");
      break;
    case T_THEN:
      printf("then");
      break;
    case T_ELSE:
      printf("else");
      break;
    case T_LEFT_PAREN:
      printf("(");
      break;
    case T_RIGHT_PAREN:
      printf(")");
      break;
    case T_EOF:
      printf("eof");
      break;
  }

  printf(" row: %lld col: %lld\n", token->line, token->col);
}
