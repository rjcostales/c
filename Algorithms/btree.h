#include <stdio.h>
#include <stdlib.h>

typedef char identifier[32];

typedef struct node {
  identifier name;
  identifier type;
  struct node *left, *right;
} node, *tree;

int insert (tree *, char[]);
node *locate (tree, char[]);
void dispose (tree *);
void print (tree);
