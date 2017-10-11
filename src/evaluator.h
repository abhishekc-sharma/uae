#ifndef UAE_EVALUATOR_H
#define UAE_EVALUATOR_H

#include "ast.h"

typedef enum {
  EVAL_SUCCESS,
  EVAL_ERROR
} evaluation_status_t;

/*typedef struct {
  evaluation_status_t status;

} evaluation_result_t;*/

evaluation_status_t evaluate(astnode_t **);

#endif
