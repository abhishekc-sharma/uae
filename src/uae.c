#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include "parser.h"
#include "ast.h"
#include "evaluator.h"

astnode_t *program = NULL;
parser_t *parser = NULL;

void handle_exit(int signo) {
  if(parser != NULL) {
    parser_delete(&parser);
  }
  if(program != NULL) {
    astnode_delete(&program, PRESERVE_NONE);
  }
  exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
  if(argc < 2) {
    // REPL Mode
    signal(SIGINT, handle_exit);
    while(1) {
      printf("> ");
      parser = parser_new(stdin);
      program = parse_term(parser);
      if(program->type == N_ERROR) {
        printf("%s", parser_error_message);
      } else {
        evaluation_status_t status = evaluate(&program);
        if(status == EVAL_ERROR) {
          printf("Evaluation Error!\n");
        } else {
          astnode_print(program);
        }
      }
      astnode_delete(&program, PRESERVE_NONE);
      parser_delete(&parser);
    }
  } else if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
    printf("Usage: uae (REPL)\n       uae <filename>\n");
  } else {
    // File Mode
    char *filename = argv[1];
    FILE *fp = fopen(filename, "r");
    parser = parser_new(fp);
    program = parse_term(parser);
    if(program->type == N_ERROR) {
      printf("%s", parser_error_message);
    } else {
      evaluation_status_t status = evaluate(&program);
      if(status == EVAL_ERROR) {
        printf("Evaluation Error!\n");
      } else {
        astnode_print(program);
      }
    }
    astnode_delete(&program, PRESERVE_NONE);
    parser_delete(&parser);
    fclose(fp);
  }
  return 0;
}
