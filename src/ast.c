#include "ast.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

astnode_t *astnode_new(node_type_t type, ...) {
  va_list ap;
  va_start(ap, type);
  astnode_t *node = malloc(sizeof(astnode_t));
  node->type = type;

  switch(type) {
    case N_IFTELSE:
      node->children = malloc(3 * sizeof(astnode_t *));
      node->children[0] = va_arg(ap, astnode_t *);
      node->children[1] = va_arg(ap, astnode_t *);
      node->children[2] = va_arg(ap, astnode_t *);
      break;
    case N_SUCC:
    case N_PRED:
    case N_ISZERO:
      node->children = malloc(1 * sizeof(astnode_t *));
      node->children[0] = va_arg(ap, astnode_t *);
      break;
    default:
      node->children = NULL;
      break;
  }

  va_end(ap);
  return node;
}

void astnode_delete(astnode_t **node_ptr, node_preservation_t preserve) {
  astnode_t *node = *node_ptr;

  switch(node->type) {
    case N_IFTELSE:
      preserve != PRESERVE_SECOND ? astnode_delete(&node->children[2], PRESERVE_NONE) : 0;
      preserve != PRESERVE_FIRST ? astnode_delete(&node->children[1], PRESERVE_NONE) : 0;
    case N_SUCC:
    case N_PRED:
    case N_ISZERO:
      preserve != PRESERVE_ZEROTH ? astnode_delete(&node->children[0], PRESERVE_NONE) : 0;
      free(node->children);
      break;
    default:
      break;
  }

  free(node);
  *node_ptr = NULL;
}

void print_indent(int level) {
  for(int i = 0; i < level; i++) printf("\t");
}

void astnode_print_level(astnode_t *node, int level) {
  switch(node->type) {
    case N_TRUE:
      print_indent(level);
      printf("TRUE\n");
      break;
    case N_FALSE:
      print_indent(level);
      printf("FALSE\n");
      break;
    case N_ZERO:
      print_indent(level);
      printf("ZERO\n");
      break;
    case N_SUCC:
      print_indent(level);
      printf("SUCC\n");
      astnode_print_level(node->children[0], level + 1);
      break;
    case N_PRED:
      print_indent(level);
      printf("PRED\n");
      astnode_print_level(node->children[0], level + 1);
      break;
    case N_ISZERO:
      print_indent(level);
      printf("ISZERO\n");
      astnode_print_level(node->children[0], level + 1);
      break;
    case N_IFTELSE:
      print_indent(level);
      printf("IF\n");
      astnode_print_level(node->children[0], level + 1);
      print_indent(level);
      printf("THEN\n");
      astnode_print_level(node->children[1], level + 1);
      print_indent(level);
      printf("ELSE\n");
      astnode_print_level(node->children[2], level + 1);
      break;
  }
}

void astnode_print(astnode_t *node) {
  astnode_print_level(node, 0);
}
