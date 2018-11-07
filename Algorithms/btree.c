#include <stdio.h>
#include <string.h>
#include "btree.h"

int
insert (tree * root, char *name) {
  if (*root) {
    if (strcmp (name, (*root)->name) < 0)
      return insert (&((*root)->left), name);
    else if (strcmp (name, (*root)->name) > 0)
      return insert (&((*root)->right), name);
    else
      return 0;
  } else {
    *root = (tree) malloc ((size_t) sizeof (node));

    strcpy ((*root)->name, name);
    (*root)->left = NULL;
    (*root)->right = NULL;
    return 1;
  }
}

node *
locate (tree root, char name[]) {
  if (!root)
    return NULL;

  if (strcmp (root->name, name) > 0)
    return locate (root->left, name);

  if (!strcmp (name, root->name))
    return root;

  if (strcmp (root->name, name) < 0)
    return locate (root->right, name);
}

void
dispose (tree * root) {
  if (!root)
    return;

  if ((*root)->left)
    dispose (&((*root)->left));

  if ((*root)->right)
    dispose (&((*root)->right));

  free (*root);
  (*root) = NULL;
}

void
print (tree root) {
  if (!root)
    return;

  if (root->left)
    print (root->left);

  printf ("%s ", root->name);

  if (root->right)
    print (root->right);
}

tree test = NULL;

main () {
  node *temp;
  int i;

  i = insert (&test, "one");
  i = insert (&test, "two");
  i = insert (&test, "three");
  i = insert (&test, "four");
  i = insert (&test, "five");
  i = insert (&test, "six");
  i = insert (&test, "seven");
  i = insert (&test, "eight");
  i = insert (&test, "nine");
  i = insert (&test, "ten");

  print (test);

  temp = locate (test, "one");
  strcpy (temp->name, "onesy");

  puts ("");
  print (test);
}
