#include "evaluator.h"
#include "ast.h"

int is_numeric(astnode_t *node) {
  if(node->type == N_ZERO || (node->type == N_SUCC && is_numeric(node->children[0]))) {
    return 1;
  }

  return 0;
}

int is_value(astnode_t *node) {
  if(node->type == N_TRUE || node->type == N_FALSE || is_numeric(node)) {
    return 1;
  }

  return 0;
}

evaluation_status_t evaluate_single_step(astnode_t *node, astnode_t **result) {
  if(node->type == N_IFTELSE) {
    if(node->children[0]->type == N_TRUE) {
      *result = node->children[1];
      astnode_delete(&node, PRESERVE_FIRST);
      return EVAL_SUCCESS;
    } else if(node->children[0]->type == N_FALSE) {
      *result = node->children[2];
      astnode_delete(&node, PRESERVE_SECOND);
      return EVAL_SUCCESS;
    } else {
      return evaluate_single_step(node->children[0], &node->children[0]);
    }
  } else if(node->type == N_SUCC) {
    return evaluate_single_step(node->children[0], &node->children[0]);
  } else if(node->type == N_PRED) {
    if(node->children[0]->type == N_ZERO) {
      *result = node->children[0];
      astnode_delete(&node, PRESERVE_ZEROTH);
      return EVAL_SUCCESS;
    } else if(node->children[0]->type == N_SUCC && is_numeric(node->children[0]->children[0])) {
      *result = node->children[0]->children[0];
      astnode_delete(&node->children[0], PRESERVE_ZEROTH);
      astnode_delete(&node, PRESERVE_ZEROTH);
      return EVAL_SUCCESS;
    } else {
      return evaluate_single_step(node->children[0], &node->children[0]);
    }
  } else if(node->type == N_ISZERO) {
    if(node->children[0]->type == N_ZERO) {
      *result = astnode_new(N_TRUE);
      astnode_delete(&node, PRESERVE_NONE);
      return EVAL_SUCCESS;
    } else if(node->children[0]->type == N_SUCC && is_numeric(node->children[0]->children[0])) {
      *result = astnode_new(N_FALSE);
      astnode_delete(&node, PRESERVE_NONE);
      return EVAL_SUCCESS;
    } else {
      return evaluate_single_step(node->children[0], &node->children[0]);
    }
  }

  return EVAL_ERROR;

}

evaluation_status_t evaluate(astnode_t **program) {
  switch (evaluate_single_step(*program, program)) {
    case EVAL_SUCCESS:
      return evaluate(program);
      break;
    case EVAL_ERROR:
      if(is_value(*program)) {
        return EVAL_SUCCESS;
      } else {
        return EVAL_ERROR;
      }
      break;
  }
}
