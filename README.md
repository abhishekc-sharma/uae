# UAE

A work in progress interpreter implemented in C for a simple language consisting of booleans, numbers and simple operations on them.

This is an implementation of the language used in Chapter 3 and 4 of Types and Programming Languages - Benjamin C. Pierce

## Grammer

```

term ::=
  true
  false
  0
  succ(term)
  pred(term)
  iszero(term)
  if term then term else term

```
