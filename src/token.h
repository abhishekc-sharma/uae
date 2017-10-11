#ifndef UAE_TOKEN_H
#define UAE_TOKEN_H

#define MAX_TOKEN_LENGTH 6

typedef enum {
  T_TRUE,
  T_FALSE,
  T_ZERO,
  T_SUCC,
  T_PRED,
  T_ISZERO,
  T_IF,
  T_THEN,
  T_ELSE,
  T_LEFT_PAREN,
  T_RIGHT_PAREN,
  T_EOF
} token_type_t;

typedef struct {
  token_type_t type;
  long long line;
  long long col;
} token_t;

token_t *token_new(token_type_t, long long, long long);
void token_delete(token_t **);
void print_token(token_t *);

int token_is_value(token_t *);
int token_is_keyword(token_t *);
int token_is_seperator(token_t *);
int token_is(token_t *, token_type_t);
#endif
