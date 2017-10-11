#ifndef UAE_AST_H
#define UAE_AST_H



typedef enum {
  N_TRUE,
  N_FALSE,
  N_ZERO,
  N_SUCC,
  N_PRED,
  N_ISZERO,
  N_IFTELSE,
  N_ERROR
} node_type_t;

typedef struct astnode {
  node_type_t type;
  struct astnode **children;
} astnode_t;

typedef enum {
  PRESERVE_NONE,
  PRESERVE_ZEROTH,
  PRESERVE_FIRST,
  PRESERVE_SECOND
} node_preservation_t;

astnode_t *astnode_new(node_type_t, ...);
void astnode_delete(astnode_t **, node_preservation_t);
void astnode_print(astnode_t *);

#endif
