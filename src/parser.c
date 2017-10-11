#include "scanner.h"
#include "token.h"
#include "parser.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

char parser_error_message[MAX_PARSER_ERROR_MSG_SIZE];

parser_t *parser_new(FILE *fp) {
  parser_t *parser = (parser_t *) malloc(sizeof(parser_t));
  parser->scanner = scanner_new(fp);
  parser->saved = NULL;

  return parser;
}

void parser_delete(parser_t **parser_ptr) {
    parser_t *parser = *parser_ptr;
    scanner_delete(&parser->scanner);
    if(parser->saved != NULL) {
      token_delete(&(parser->saved));
    }
    free(*parser_ptr);
    *parser_ptr = NULL;
}

token_t *parser_next_token(parser_t *parser) {
  token_t *next_token;
  if(parser->saved != NULL) {
    next_token = parser->saved;
    parser->saved = NULL;
  } else {
    next_token = scanner_token(parser->scanner);
  }

  return next_token;
}

astnode_t *parse_term(parser_t *parser) {
  token_t *next_token = parser_next_token(parser);

  if(next_token == NULL) {
    return astnode_new(N_ERROR);
  }

  astnode_t *root = NULL;

  if(token_is(next_token, T_TRUE)) {
    root = astnode_new(N_TRUE);
  } else if(token_is(next_token, T_FALSE)) {
    root = astnode_new(N_FALSE);
  } else if(token_is(next_token, T_ZERO)) {
    root = astnode_new(N_ZERO);
  } else if(token_is(next_token, T_SUCC)) {
    token_t *l_paren_token = parser_next_token(parser);
    if(token_is(l_paren_token, T_LEFT_PAREN)) {
      astnode_t *arg = parse_term(parser);
      if(arg->type == N_ERROR) {
        root = arg;
      } else {
        token_t *r_paren_token = parser_next_token(parser);
        if(token_is(r_paren_token, T_RIGHT_PAREN)) {
          root = astnode_new(N_SUCC, arg);
        } else {
          root = astnode_new(N_ERROR);
          sprintf(parser_error_message, "Expected )\n");
          astnode_delete(&arg, PRESERVE_NONE);
        }
        token_delete(&r_paren_token);
      }
    } else {
      root = astnode_new(N_ERROR);
      sprintf(parser_error_message, "Expected (\n");
    }
    token_delete(&l_paren_token);
  } else if(token_is(next_token, T_PRED)) {
    token_t *l_paren_token = parser_next_token(parser);
    if(token_is(l_paren_token, T_LEFT_PAREN)) {
      astnode_t *arg = parse_term(parser);
      if(arg->type == N_ERROR) {
        root = arg;
      } else {
        token_t *r_paren_token = parser_next_token(parser);
        if(token_is(r_paren_token, T_RIGHT_PAREN)) {
          root = astnode_new(N_PRED, arg);
        } else {
          root = astnode_new(N_ERROR);
          sprintf(parser_error_message, "Expected )\n");
          astnode_delete(&arg, PRESERVE_NONE);
        }
        token_delete(&r_paren_token);
      }
    } else {
      root = astnode_new(N_ERROR);
      sprintf(parser_error_message, "Expected (\n");
    }
    token_delete(&l_paren_token);
  } else if(token_is(next_token, T_ISZERO)) {
    token_t *l_paren_token = parser_next_token(parser);
    if(token_is(l_paren_token, T_LEFT_PAREN)) {
      astnode_t *arg = parse_term(parser);
      if(arg->type == N_ERROR) {
        root = arg;
      } else {
        token_t *r_paren_token = parser_next_token(parser);
        if(token_is(r_paren_token, T_RIGHT_PAREN)) {
          root = astnode_new(N_ISZERO, arg);
        } else {
          root = astnode_new(N_ERROR);
          sprintf(parser_error_message, "Expected )\n");
          astnode_delete(&arg, PRESERVE_NONE);
        }
        token_delete(&r_paren_token);
      }
    } else {
      root = astnode_new(N_ERROR);
      sprintf(parser_error_message, "Expected (\n");
    }
    token_delete(&l_paren_token);
  } else if(token_is(next_token, T_IF)) {
    astnode_t *cond = parse_term(parser);
    if(cond->type == N_ERROR) {
      root = cond;
    } else {
      token_t *then_token = parser_next_token(parser);
      if(token_is(then_token, T_THEN)) {
        astnode_t *true_exp = parse_term(parser);
        if(true_exp->type == N_ERROR) {
          root = true_exp;
          astnode_delete(&cond, PRESERVE_NONE);
        } else {
          token_t *else_token = parser_next_token(parser);
          if(token_is(else_token, T_ELSE)) {
            astnode_t *false_exp = parse_term(parser);
            if(false_exp->type == N_ERROR) {
              root = false_exp;
              astnode_delete(&cond, PRESERVE_NONE);
              astnode_delete(&true_exp, PRESERVE_NONE);
            } else {
              root = astnode_new(N_IFTELSE, cond, true_exp, false_exp);
            }
          } else {
            root = astnode_new(N_ERROR);
            sprintf(parser_error_message, "Expected keyword 'else'\n");

          }
          token_delete(&else_token);
        }
      } else {
        root = astnode_new(N_ERROR);
        sprintf(parser_error_message, "Expected keyword 'then'\n");
      }
      token_delete(&then_token);
    }
  } else {
    root = astnode_new(N_ERROR);
    sprintf(parser_error_message, "Unexpected token\n");
    //scanner_flush_line(parser->scanner);
  }

  token_delete(&next_token);
  return root;
}
