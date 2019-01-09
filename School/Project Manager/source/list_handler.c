/* Module: list_handler.c
 *
 * Purpose: The main purpose of this module is provide functions to deal with and maintain the
 * list data structures.
 *
 * Written by Jesse Costales, Ismael Flores, Julian Escontrias, and Mike Marez.
 *
 */

#include "project_manager.h"

extern list attribute_names;
extern list relation_names;
extern list undeclared;

extern ot_tree object_types;
extern rd_tree relation_defs;

extern object Object_Table[];
extern relationship Relation_Table[];

int list_insert_name(list *head, char *name)
{
    list_node      *temp;
    int i;

    if (*head)
    {
        if ((i = strcmp(name, (*head)->name)) == 0)
            return FALSE;
        else if (i > 0)
            return list_insert_name(&((*head)->next), name);
    }

    temp = (*head);

    (*head) = (list) malloc((size_t) sizeof(list_node));

    if (*head)
    {
        strcpy((*head)->name, name);
        (*head)->text = NULL;
        (*head)->next = temp;

        return TRUE;
    }
    else
    {
        printf("FATAL ERROR - UNABLE TO ALLOCATE MEMORY\n");
        return FALSE;
    }
}

int list_insert_text(list *head, char *name, char *text)
{
    list_node      *temp;
    int i;

    if (*head)
    {
        if ((i = strcmp(name, (*head)->name)) == 0)
            return FALSE;
        else if (i > 0)
            return list_insert_text(&((*head)->next), name, text);
    }

    temp = (*head);

    (*head) = (list) malloc((size_t) sizeof(list_node));

    if ((*head) && ((*head)->text = (char *) malloc((size_t) (strlen(text) + 1))))
    {
        strcpy((*head)->name, name);
        strcpy((*head)->text, text);
        (*head)->next = temp;
    }
    else
    {
        printf("FATAL ERROR - UNABLE TO ALLOCATE MEMORY\n");
        return FALSE;
    }
    return FALSE;
}

char *list_get_name(list head, int n)
{
    int i;

    for (i = 0; i < n; i++)
        head = head->next;

    return head->name;
}

char *list_get_text(list head, char *name)
{
    while (head)
    {
        if (!strcmp(head->name, name))
            return head->text;

        head = head->next;
    }
    return NULL;
}

void list_delete(list *head, char *name)
{
    list_node      *temp;

    if (*head)
    {
        if (strcmp(name, (*head)->name) == 0)
        {
            temp = (*head)->next;
            free((*head)->name);
            free((*head)->text);
            free(*head);
            (*head) = temp;
        }
        else
            list_delete(&((*head)->next), name);
    }
}

list list_locate(list head, char *name)
{
    if (head)
    {
        if (strcmp(name, head->name) == 0)
            return head;
        else
            return list_locate(head->next, name);
    }
    return NULL;
}

int list_index(list head, char *name)
{
    int i;

    for (i = 0; head; i++)
    {
        if (!strcmp(head->name, name))
            return i;
        head = head->next;
    }

    return -1;
}

void list_dispose(list *head)
{
    if (*head)
    {
        list_dispose(&((*head)->next));

        if ((*head)->name)
            free((*head)->name);

        if ((*head)->text)
            free((*head)->text);

        free(*head);
        (*head) = NULL;
    }
}

int list_length(list head)
{
    int i;

    for (i = 0; head; i++)
        head = head->next;

    return i;
}

int obj_type_insert(ot_tree *root, char *type)
{
    int i;

    if (*root)
    {
#ifdef DEBUG
        printf("finding\n");
#endif
        if (!(i = strcmp(type, (*root)->type)))
            return FALSE;

        else if (i < 0)
            return obj_type_insert(&((*root)->left), type);

        else if (i > 0)
            return obj_type_insert(&((*root)->right), type);
    }
    else
    {
        (*root) = (ot_tree) malloc((size_t) sizeof(obj_type_node));

        if (*root)
        {
            strcpy((*root)->type, type);
            (*root)->att_list = NULL;
            (*root)->rel_list = NULL;
            (*root)->right = NULL;
            (*root)->left = NULL;

#ifdef DEBUG
            printf("adding\n");
#endif
            return TRUE;
        }
        else
        {
            printf("FATAL ERROR - UNABLE TO ALLOCATE MEMORY\n");
            return FALSE;
        }
    }
    return FALSE;
}

ot_tree obj_type_locate(ot_tree root, char *type)
{
    int i;

    if (root)
    {
        if (!(i = strcmp(type, root->type)))
            return root;

        else if (i < 0)
            return obj_type_locate(root->left, type);

        else if (i > 0)
            return obj_type_locate(root->right, type);
    }
    return NULL;
}

void obj_type_dispose(ot_tree *root)
{
    if (*root)
    {
        if ((*root)->left)
            obj_type_dispose(&((*root)->left));

        if ((*root)->right)
            obj_type_dispose(&((*root)->right));

        list_dispose(&((*root)->att_list));
        list_dispose(&((*root)->rel_list));

        free(*root);
        (*root) = NULL;
    }
}

int rel_def_insert(rd_tree *root, char *domain, char *relation, char *range, char *inverse)
{
    int i;

    if (*root)
    {
        if (!(i = strcmp(domain, (*root)->domain)))
            if (!(i = strcmp(relation, (*root)->relation)))
                if (!(i = strcmp(range, (*root)->range)))
                    return FALSE;
        if (i < 0)
            return rel_def_insert(&((*root)->left), domain, relation, range, inverse);

        if (i > 0)
            return rel_def_insert(&((*root)->right), domain, relation, range, inverse);
    }
    else
    {
        (*root) = (rd_tree) malloc((size_t) sizeof(rel_def_node));

        if (*root)
        {
            strcpy((*root)->domain, domain);
            strcpy((*root)->relation, relation);
            strcpy((*root)->range, range);
            strcpy((*root)->inverse, inverse);
            (*root)->right = NULL;
            (*root)->left = NULL;

            return TRUE;
        }
        else
        {
            printf("FATAL ERROR - UNABLE TO ALLOCATE MEMORY\n");
            return FALSE;
        }
    }
    return FALSE;
}

rd_tree rel_def_locate(rd_tree root, char *domain, char *relation, char *range)
{
    int i;

    if (root)
    {
        if (!(i = strcmp(domain, root->domain)))
            if (!(i = strcmp(relation, root->relation)))
                if (!(i = strcmp(range, root->range)))
                    return root;

        if (i < 0)
            return rel_def_locate(root->left, domain, relation, range);

        if (i > 0)
            return rel_def_locate(root->right, domain, relation, range);
    }
    return NULL;
}

void rel_def_dispose(rd_tree *root)
{
    if (*root)
    {
        if ((*root)->left)
            rel_def_dispose(&((*root)->left));

        if ((*root)->right)
            rel_def_dispose(&((*root)->right));

        free(*root);
        (*root) = NULL;
    }
}
