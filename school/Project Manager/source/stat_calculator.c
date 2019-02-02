/* Module: stat_calculator.c
 *
 * Purpose: This module performs the metrics calculations for the controller module.
 *
 * Written by Jesse Costales, Ismael Flores, Julian Escontrias, and Mike Marez.
 *
 * Note: This module contains a dynamically allocated structure called adj_matrix and is a n x n
 * matrix of boolean values, where n is the number of processes.
 *
 */

#include "project_manager.h"

extern FILE    *output, *input;

extern list attribute_names;
extern list relation_names;
extern list undeclared;

extern ot_tree object_types;
extern rd_tree relation_defs;

extern object Object_Table[];
extern relationship Relation_Table[];

matrix matrix_new(int s)
{
    int i, *temp;

    temp = (matrix) malloc((size_t) s * s);

    i = s * s;

    while (i--)
        temp[i] = 0;

    return temp;
}

void matrix_clear(matrix m, int s)
{
    int i;

    i = s * s;
    while (i--)
        m[i] = 0;
}

void matrix_set(int i, int j, matrix m, int s)
{
    m[(i * s) + j] |= 1;
}

void matrix_copy(matrix m, matrix n, int s)
{
    int i;

    i = s * s;
    while (i--)
        m[i] = n[i];
}

void matrix_product(matrix m, matrix n, matrix o, int s)
{
    int i, j, k;

    matrix_clear(m, s);

    for (i = 0; i < s; i++)
        for (j = 0; j < s; j++)
            for (k = 0; k < s; k++)
                m[(i * s) + k] |= n[(i * s) + j] && o[(j * s) + k];
}

int matrix_circuit(matrix m, int s)
{
    int i;

    i = s;

    while (i--)
        if (m[(i * s) + i])
            return TRUE;

    return FALSE;
}

int matrix_zero(matrix m, int s)
{
    int i, j;

    for (i = 0; i < s; i++)
        for (j = 0; j < s; j++)
            if (m[(i * s) + j])
                return FALSE;

    return TRUE;
}

void matrix_print(matrix m, int s)
{
    int i, j;

    for (i = 0; i < s; i++)
    {
        for (j = 0; j < s; j++)
            printf("%2d ", m[(i * s) + j]);

        printf("\n");
    }
    printf("\n");
}

void compute_levels(obj_type type, rel_name relation)
{
    int i, s = 1;
    list process_list = NULL;
    list temp;
    matrix init, prev, curr;

    output = stdout;

    printf("\n Computing Levels\n\n");

    for (i = 0; Relation_Table[i]; i++)
    {
        if (!strcmp(type, find_type(Relation_Table[i]->subject)) &&
            !strcmp(relation, Relation_Table[i]->relation))
        {
            temp = Relation_Table[i]->obj_list;
            list_insert_name(&process_list, Relation_Table[i]->subject);
            while (temp)
            {
                list_insert_name(&process_list, temp->name);
                temp = temp->next;
            }
        }
    }

    s = list_length(process_list);
    init = matrix_new(s);
    prev = matrix_new(s);
    curr = matrix_new(s);

    for (i = 0; Relation_Table[i]; i++)
    {
        if (!strcmp(type, find_type(Relation_Table[i]->subject)) &&
            !strcmp(relation, Relation_Table[i]->relation))
        {
            int length;

            temp = Relation_Table[i]->obj_list;

            printf(" %s %s contains %d %s", type, Relation_Table[i]->subject, length = list_length(temp), relation);

            if (length > 7)
                printf(", exceeds recommended number of calls!\n");
            else
                printf(".\n");

            /*	set values of the adjacency matrix */
            while (temp)
            {
                matrix_set(list_index(process_list, temp->name),
                           list_index(process_list, Relation_Table[i]->subject),
                           init, s);
                temp = temp->next;
            }
        }
    }

    matrix_copy(prev, init, s);

    for (i = 1; i < s; i++)
    {
        matrix_product(curr, prev, init, s);
        matrix_copy(prev, curr, s);

        if (matrix_zero(curr, s))
        {
            printf("\n Max. depth = %d\n", i);

            break;
        }

        if (matrix_circuit(curr, s))
        {
            i = s;

            while (i--)
                if (curr[(i * s) + i])
                    list_insert_name(&temp, list_get_name(process_list, i));

            printf("\n Warning - the following object form a circuit:");
            list_print_name(temp);
            printf("!\n");

            break;
        }
    }

    printf("\n Number of Objects by Type\n\n");
    obj_type_count_print(object_types);
}

void compute_calls(obj_type type, rel_name relation)
{
    int i;
    list temp;

    output = stdout;

    printf("\n Computing Module Calls\n\n");

    for (i = 0; Relation_Table[i]; i++)
    {
        if (!strcmp(type, find_type(Relation_Table[i]->subject)) &&
            !strcmp(relation, Relation_Table[i]->relation))
        {
            int length;

            temp = Relation_Table[i]->obj_list;

            printf(" %s %s %s %d modules", type, Relation_Table[i]->subject, relation, length = list_length(temp));

            if (length > 10)
                printf(", exceeds recommended number of calls!\n");
            else
                printf(".\n");
        }
    }
}

void compute_complexity(obj_type type, att_name path)
{
    int i, line_count, region_count, total_line_count = 0, total_region_count = 0;
    char           *filename;

    output = stdout;

    printf("\n Computing Complexity\n\n");

    for (i = 0; Object_Table[i]; i++)
    {
        if (!strcmp(type, Object_Table[i]->type))
        {
            line_count = 0;
            region_count = 0;

            filename = list_get_text(Object_Table[i]->attributes, path);

            input = fopen(filename,"r");

            if (input)
            {
                Macabe(&line_count, &region_count);
                printf(" %s contains %d lines and %d regions.\n", filename, line_count, region_count);
            }
            else
                printf(" CAN'T OPEN %s\n", filename);

            total_line_count += line_count;
            total_region_count += region_count;
        }
    }
    printf("\n Total line count = %d\n Total region count = %d\n", total_line_count, total_region_count);
}
