#include "list.h"

void
insert (list * head, char *name) {
  node *temp;

  if (((*head) == NULL) || (strcmp (name, (*head)->name) < 0)) {
    temp = (*head);

    (*head) = (node *) malloc ((size_t) sizeof (node));

    (*head)->next = temp;
    (*head)->text = NULL;

    strcpy ((*head)->name, name);
  } else
    insert (&((*head)->next), name);
}

void
append (list * head, char *name) {
  if (*head)
    append (&((*head)->next), name);
  else {
    (*head) = (node *) malloc ((size_t) sizeof (node));

    (*head)->next = NULL;
    (*head)->text = NULL;

    strcpy ((*head)->name, name);
  }
}

void
delete (list * head, char *name) {
  node *temp;

  if (*head) {
    if (strcmp (name, (*head)->name) == 0) {
      temp = (*head)->next;

      if ((*head)->text)
        free ((*head)->text);

      free (*head);

      (*head) = temp;
    } else
      delete (&((*head)->next), name);
  }
}

node *
locate (list head, char *name) {
  if (head) {
    if (strcmp (name, head->name) == 0)
      return head;
    else
      return locate (head->next, name);
  }
  return NULL;
}

void
dispose (list * head) {
  if (*head) {
    dispose (&((*head)->next));

    if ((*head)->text)
      free ((*head)->text);

    free (*head);

    (*head) = NULL;
  }
}

int
length (list head) {
  int i;

  for (i = 0; head != NULL; i++)
    head = head->next;

  return i;
}

void
print (list head) {
  if (head) {
    printf ("%s\t\"%s\"%s", head->name, head->text,
            (head->next) ? ";\n" : ".\n");

    print (head->next);
  }
}

main () {
  list test = NULL;
  node *temp;

  insert (&test, "process");
  insert (&test, "file");
  insert (&test, "document");

  print (test);

  temp = locate (test, "process");
  temp->text = malloc (5);
  strcpy (temp->text, "path");

  print (test);

  delete (&test, "process");
  print (test);

  insert (&test, "process");
  print (test);

  dispose (&test);

}
