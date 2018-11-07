#include <stdio.h>
#include <stdlib.h>

typedef char identifier[32];

typedef struct node {
  identifier name;
  char *text;
  struct node *next;
} node, *list;

void insert (list *, char[]);
void append (list *, char[]);
void delete (list *, char[]);

node *locate (list, char *);

void dispose (list *);

int length (list);
void print (list);
