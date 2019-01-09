/* Module: table_handler.c
 *
 * Purpose: The main purpose of this module is provide functions to deal with and maintain the
 * table data structure.
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

int obj_insert(object *table, obj_name name, obj_type type)
{
    int i = 0;

    while (table[i])
    {
        if (!strcmp(table[i]->name, name))
            return i;
        else
            i++;
    }

    if (i < MAX_OBJECTS)
    {
        table[i] = (object) malloc((size_t) sizeof(obj_node));

        if (table[i])
        {
            strcpy(table[i]->name, name);
            strcpy(table[i]->type, type);
            table[i]->attributes = NULL;

            return i;
        }
        else
        {
            printf("FATAL ERROR - UNABLE TO ALLOCATE MEMORY\n");
            return 0;
        }
    }
    else
    {
        printf("FATAL ERROR - TOO MANY OBJECTS\n");
        return 0;
    }
}

int obj_size(object *table)
{
    int i = 0;

    while (table[i++]);

    return i - 1;
}

int obj_count_type(object *table, char *type)
{
    int i = 0, count = 0;

    while (table[i])
        if (!strcmp(table[i++]->type, type))
            count++;

    return count;
}

void obj_swap(object *table, int i, int j)
{
    object temp;

    temp = table[i];
    table[i] = table[j];
    table[j] = temp;
}

void obj_sort_name(object *table, int left, int right)
{
    int i, last;

    if (left >= right)
        return;

    obj_swap(table, left, (left + right) / 2);
    last = left;

    for (i = left + 1; i <= right; i++)
        if (strcmp(table[i]->name, table[left]->name) < 0)
            obj_swap(table, ++last, i);

    obj_swap(table, left, last);
    obj_sort_name(table, left, last - 1);
    obj_sort_name(table, last + 1, right);
}

void obj_sort_type(object *table, int left, int right)
{
    int i, last, c;

    if (left >= right)
        return;

    obj_swap(table, left, (left + right) / 2);
    last = left;

    for (i = left + 1; i <= right; i++)
        if ((c = strcmp(table[i]->type, table[left]->type)) < 0)
            obj_swap(table, ++last, i);
        else if ((c == 0) && (strcmp(table[i]->name, table[left]->name) < 0))
            obj_swap(table, ++last, i);

    obj_swap(table, left, last);
    obj_sort_type(table, left, last - 1);
    obj_sort_type(table, last + 1, right);
}

void obj_dispose(object *table)
{
    int i = 0;

    while (table[i])
    {
        if (table[i]->attributes)
            list_dispose(&(table[i]->attributes));

        free(table[i]);
        table[i] = NULL;

        i++;
    }
}

int rel_insert(relationship *table, obj_name subject, rel_name relation)
{
    int i = 0;

    while (table[i])
    {
        if ((!strcmp(table[i]->subject, subject)) && (!strcmp(table[i]->relation, relation)))
            return i;
        else
            i++;
    }
    if (i < MAX_RELATIONS)
    {
        table[i] = (relationship) malloc((size_t) sizeof(rel_node));

        if (table[i])
        {
            strcpy(table[i]->subject, subject);
            strcpy(table[i]->relation, relation);
            table[i]->obj_list = NULL;

            return i;
        }
        else
        {
            printf("FATAL ERROR - UNABLE TO ALLOCATE MEMORY\n");
            return 0;
        }
    }
    else
    {
        printf("FATAL ERROR - TOO MANY RELATIONS\n");
        return 0;
    }
}

int rel_size(relationship *table)
{
    int i = 0;

    while (table[i++]);

    return i - 1;
}

void rel_swap(relationship *table, int i, int j)
{
    long temp;

    temp = (long) table[i];
    table[i] = table[j];
    table[j] = (relationship) temp;
}

void rel_sort_relation(relationship *table, int left, int right)
{
    int i, last, c;

    if (left >= right)
        return;

    rel_swap(table, left, (left + right) / 2);
    last = left;

    for (i = left + 1; i <= right; i++)
        if ((c = strcmp(table[i]->relation, table[left]->relation)) < 0)
            rel_swap(table, ++last, i);
        else if ((c == 0) && (strcmp(table[i]->subject, table[left]->subject) < 0))
            rel_swap(table, ++last, i);

    rel_swap(table, left, last);
    rel_sort_relation(table, left, last - 1);
    rel_sort_relation(table, last + 1, right);
}

void rel_sort_object(relationship *table, int left, int right)
{
    int i, last, c;

    if (left >= right)
        return;

    rel_swap(table, left, (left + right) / 2);
    last = left;

    for (i = left + 1; i <= right; i++)
        if ((c = strcmp(table[i]->subject, table[left]->subject)) < 0)
            rel_swap(table, ++last, i);
        else if ((c == 0) && (strcmp(table[i]->relation, table[left]->relation) < 0))
            rel_swap(table, ++last, i);

    rel_swap(table, left, last);
    rel_sort_object(table, left, last - 1);
    rel_sort_object(table, last + 1, right);
}

void rel_dispose(relationship *table)
{
    int i = 0;

    while (table[i])
    {
        if (table[i]->obj_list)
            list_dispose(&(table[i]->obj_list));

        free(table[i]);
        table[i] = NULL;

        i++;
    }
}

int att_search(object *table, att_name name)
{
    int i = 0;

    while (table[i])
    {
        if (!strcmp(table[i]->name, name))
            return i;
        else
            i++;
    }
    return i;
}
