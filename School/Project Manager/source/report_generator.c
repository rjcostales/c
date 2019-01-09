/* Module: report_generator.c
 *
 * Purpose: The report_generator module is called by the controller to output status reports
 * depending on the call.
 *
 * Written by Jesse Costales, Ismael Flores, Julian Escontrias, and Mike Marez.
 *
 * Note: This module may directly read but not modify the Object_Table and Relation_Table global
 * data structures.	 Also, the module calls the data_manager sort routines prior to generating
 * reports.
 *
 */

#include "project_manager.h"

FILE           *output;

extern list attribute_names;
extern list relation_names;
extern list undeclared;
extern list unused_globals;
extern list unused_subroutines;

extern ot_tree object_types;
extern rd_tree relation_defs;

extern object Object_Table[];
extern relationship Relation_Table[];

void list_print_name(list head)
{
    if (head)
    {
        fprintf(output,  " %s", head->name);

        if (head->text)
            fprintf(output,  " - %s", head->text);

        if (head->next)
            fprintf(output, ",");

        list_print_name(head->next);
    }
}

void list_print_text(list head)
{
    if (head)
    {
        fprintf(output,"\t%s", head->name);

        if (head->text)
            fprintf(output," \"%s\"", head->text);

        if (head->next)
            fprintf(output,";\n");

        list_print_text(head->next);
    }
    else
        fprintf(output,".\n\n");
}

void obj_type_print(ot_tree root)
{
    if (root)
    {
        obj_type_print(root->left);

        fprintf(output,"%s", root->type);
        list_print_name(root->att_list);
        fprintf(output,";\n");

        obj_type_print(root->right);
    }
}

void obj_type_count_print(ot_tree root)
{
    if (root)
    {
        obj_type_count_print(root->left);

        fprintf(output," %s - %d\n",
                root->type,
                obj_count_type(Object_Table, root->type));

        obj_type_count_print(root->right);
    }
}

void rel_print_def(rd_tree root)
{
    if (root)
    {
        if (root->left)
            rel_print_def(root->left);

        if (!strcmp(root->relation, root->inverse) && !strcmp(root->domain, root->range))
            fprintf(output,"%s %s;\n", root->domain, root->relation);

        else if (strcmp(root->relation, root->inverse) < 0)
            fprintf(output,"%s %s/%s %s;\n", root->domain, root->relation, root->inverse, root->range);

        else if (strcmp(root->domain, root->range) < 0)
            fprintf(output,"%s %s/%s %s;\n", root->domain, root->relation, root->inverse, root->range);

        if (root->right)
            rel_print_def(root->right);
    }
}

int rel_print_list(obj_name name)
{
    int i = 0, flag = 0;

    while (Relation_Table[i])
    {
        if (!strcmp(Relation_Table[i]->subject, name))
        {
            if (!flag)
                fprintf(output,"\n");
            flag = 1;

            if (Relation_Table[i]->obj_list)
            {

                list temp = Relation_Table[i]->obj_list;
                char *rname = Relation_Table[i]->relation, end;

                if (Relation_Table[i + 1])
                    end = strcmp(Relation_Table[i + 1]->subject, name) ? '.' : ';';
                else
                    end = '.';

                while (temp)
                {
                    fprintf(output," %s\t%s%c\n", rname, temp->name, (temp->next ? ';' : end));
                    temp = temp->next;
                }
            }
        }
        i++;
    }
    return flag;
}

static list functions[16];
static list used_subroutines = NULL;

#define indent(l) { \
        int i = (l); \
        while (i--) \
            fprintf(output, "| "); \
}

void rel_print_level(char *name, int undecl, int level, int max)
{
    int i, size = rel_size(Relation_Table);
    list curr = NULL;

    if (level > 16)
    {
        fprintf(output, "** NESTING LEVEL GREATER THAN 16 **\n");
        return;
    }
    else if (level > max)
    {
        return;
    }
    else if (list_locate(undeclared, name))
    {
        if (undecl)
        {
            indent(level);
            fprintf(output, "+---external %s\n", name);
        }
        return;
    }
/*	else if (list_locate(used_subroutines, name))
    {
        indent(level);
        fprintf(output, "+---%s\n", name);
        return;
    }
 */ else for (i = 0; i < level; i++)
        {
            if (list_locate(functions[i], name))
            {
                indent(level);
                fprintf(output, "+---%s^\n", name);
                return;
            }
        }

    indent(level);
    fprintf(output, "+---%s\n", name);

    for (i = 0; i < size; i++)
    {
        if (!strcmp(Relation_Table[i]->subject, name) &&
            !strcmp(Relation_Table[i]->relation, "calls"))
        {
            curr = Relation_Table[i]->obj_list;
            break;
        }
    }

    functions[level + 1] = curr;
    list_insert_name(&used_subroutines, name);

    while (curr)
    {
        if (!strcmp(name, curr->name))
        {
            indent(level + 1);
            fprintf(output, "+---(%s)\n", curr->name);
        }
        else
        {
            rel_print_level(curr->name, undecl, level + 1, max);
        }

        curr = curr->next;
    }
}

void report_relations(char *filename)
{
    int i = 0;

    if (strlen(filename))
        output = fopen(filename, "w");
    else
        output = stdout;

    rel_sort_relation(Relation_Table, 0, rel_size(Relation_Table) - 1);

    while (Relation_Table[i])
    {
        fprintf(output, "%s\t%s", Relation_Table[i]->subject, Relation_Table[i]->relation);
        list_print_name(Relation_Table[i]->obj_list);
        fprintf(output, "\n");
        i++;
    }

    if (strlen(filename))
        fclose(output);
}

void report_obj_by_name(char *filename)
{
    int i = 0;

    if (strlen(filename))
        output = fopen(filename, "w");
    else
        output = stdout;

    fprintf(output, "\n Objects by Name\n\n");

    obj_sort_name(Object_Table, 0, obj_size(Object_Table) - 1);

    while (Object_Table[i])
    {
        fprintf(output, "%-24s%s\n", Object_Table[i]->type, Object_Table[i]->name);
        i++;
    }

    if (strlen(filename))
        fclose(output);
}

void report_obj_by_type(char *filename)
{
    int i = 0;

    if (strlen(filename))
        output = fopen(filename, "w");
    else
        output = stdout;

    fprintf(output, "\n Objects by Type\n\n");

    obj_sort_name(Object_Table, 0, obj_size(Object_Table) - 1);

    while (Object_Table[i])
    {
        fprintf(output, "%-24s%s\n", Object_Table[i]->name, Object_Table[i]->type);
        i++;
    }

    if (strlen(filename))
        fclose(output);
}

void report_vocab(char *filename)
{
    if (strlen(filename))
        output = fopen(filename, "w");
    else
        output = stdout;

    fprintf(output, "\n\* VOCABULARY %s\n\n", filename);

    obj_type_print(object_types);
    fprintf(output, "\n-\n\n");
    rel_print_def(relation_defs);

    if (strlen(filename))
        fclose(output);
}

void report_desc(char *filename)
{
    int i = 0, flag;

    if (strlen(filename))
        output = fopen(filename, "w");
    else
        output = stdout;

    fprintf(output, "\n\* DESCRIPTION %s\n\n", filename);

    obj_sort_type(Object_Table, 0, obj_size(Object_Table) - 1);

    while (Object_Table[i])
    {
        fprintf(output, "%s %s", Object_Table[i]->type, Object_Table[i]->name);

        flag = rel_print_list(Object_Table[i]->name);

        if (flag)
            fprintf(output, "\n");
        else
            fprintf(output, ".\n\n");

        if ((flag) && (Object_Table[i]->attributes))
            fprintf(output, ";\n");

        if ((flag == 0) && (Object_Table[i]->attributes))
            fprintf(output, "\n");

/*		list_print_text(Object_Table[i]->attributes); */
        i++;
    }

    if (undeclared)
    {
        fprintf(output, "* UNDECLARED OBJECTS *\n*");
        list_print_name(undeclared);
        fprintf(output, "\n\n");
    }

    if (unused_globals)
    {
        fprintf(output, "* UNUSED GLOBALS *\n*");
        list_print_name(unused_globals);
        fprintf(output, "\n\n");
    }

    if (unused_subroutines)
    {
        fprintf(output, "* UNUSED SUBROUTINES *\n*");
        list_print_name(unused_subroutines);
        fprintf(output, "\n");
    }

    if (strlen(filename))
        fclose(output);
}

void report_levels(char *filename)
{
    char function[32];
    char choice[2];
    int flag = 0;

    if (strlen(filename))
        output = fopen(filename, "w");
    else
        output = stdout;

    printf(" Include undeclared function? ");
    gets(function);

    if (tolower(*function) == 'y')
        flag = 1;

    printf(" Max. Level, default 4? ");
    gets(choice);

    if (!strlen(choice))
        strcpy(choice, "4");

    printf(" Function, default is main? ");
    gets(function);

    if (!strlen(function))
        strcpy(function, "main");

    fprintf(output, "\n\* LEVELS %s\n\n", filename);

    rel_print_level(function, flag, 0, atoi(choice));

    if (strlen(filename))
        fclose(output);
}
