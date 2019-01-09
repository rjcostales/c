/* Module: data_manager.c
 *
 * Purpose: This module updates and modifies the vocabulary and description databases. It also
 * updates the databases automatically when called by the controller module.
 *
 * Written by Jesse Costales, Ismael Flores, Julian Escontrias, and Mike Marez.
 *
 * Note: The data_manager module sorts the database tables when called by the report_generator and
 * stat_calculator modules.
 *
 */

#include "project_manager.h"

extern list attribute_names;
extern list relation_names;
extern list undeclared;
extern list unused_globals;
extern list unused_subroutines;

extern ot_tree object_types;
extern rd_tree relation_defs;

extern object Object_Table[];
extern relationship Relation_Table[];

int is_obj_type(char *object_type)
{
    if (obj_type_locate(object_types, object_type))
        return TRUE;
    else
        return FALSE;
}

int is_rel_name(char *object_type, char *relation_name)
{
    obj_type_node   *temp;

    temp = (obj_type_node *) obj_type_locate(object_types, object_type);

    if (temp)
    {
        if (list_locate((list) (temp->rel_list), relation_name))
            return TRUE;
        else
            return FALSE;
    }
    else
        return FALSE;
}

int is_att_name(char *object_type, char *attribute_name)
{
    obj_type_node  *temp;

    temp = (obj_type_node *) obj_type_locate(object_types, object_type);

    if (temp)
    {
        if (list_locate((list) (temp->att_list), attribute_name))
            return TRUE;
        else
            return FALSE;
    }
    else
        return FALSE;
}

int is_global_att_name(char *attribute_name)
{
    if (list_locate((list) attribute_names, attribute_name))
        return TRUE;
    else
        return FALSE;
}

int is_global_rel_name(char *relation_name)
{
    if (list_locate((list) relation_names, relation_name))
        return TRUE;
    else
        return FALSE;
}

int is_rel_def(char *domain, char *relation, char *range)
{
    if (rel_def_locate(relation_defs, domain, relation, range))
        return TRUE;
    else
        return FALSE;
}

int is_obj_name(char *name)
{
    int i = 0;

    while (Object_Table[i])
        if (!strcmp(Object_Table[i++]->name, name))
            return TRUE;

    return FALSE;
}

char *find_type(obj_name name)
{
    int i = 0;

    while (Object_Table[i])
    {
        if (!strcmp(Object_Table[i]->name, name))
            return (Object_Table[i]->type);
        else
            i++;
    }

    return NULL;
}

char *find_inverse(char *domain, char *relation, char *range)
{
    rel_def_node   *temp;

    temp = (rel_def_node *) rel_def_locate(relation_defs, domain, relation, range);
    return temp->inverse;
}

int add_obj_type(char *object_type)
{
    if (is_obj_type(object_type))
    {
        printf(" DEFINITION ERROR - OBJECT TYPE %s ALREADY DEFINED\n", object_type);
        return FALSE;
    }

    if (is_global_att_name(object_type))
    {
        printf(" DEFINITION ERROR - ATTRIBUTE NAME %s ALREADY DEFINED\n", object_type);
        return FALSE;
    }

#ifdef DEBUG
    printf("before\t%p\n", object_types);
    obj_type_insert(&object_types, object_type);
    printf("inserting\n");
    printf("after\t%p\n\n", object_types);
#else
    obj_type_insert(&object_types, object_type);
#endif
    return TRUE;
}

int add_rel_name(char *object_type, char *relation_name)
{
    obj_type_node  *temp;

    if (is_obj_type(relation_name))
    {
        printf(" DEFINITION ERROR - OBJECT TYPE %s ALREADY DEFINED\n", relation_name);
        return FALSE;
    }

    if (is_global_att_name(relation_name))
    {
        printf(" DEFINITION ERROR - ATTRIBUTE NAME %s ALREADY DEFINED\n", relation_name);
        return FALSE;
    }

    temp = (obj_type_node *) obj_type_locate(object_types, object_type);

    if (temp)
    {
        list_insert_name(&(temp->rel_list), relation_name);
        return TRUE;
    }
    else
        return FALSE;
}

int add_att_name(char *object_type, char *attribute_name)
{
    obj_type_node  *temp;

    if (is_obj_type(attribute_name))
    {
        printf(" DEFINITION ERROR - OBJECT TYPE %s ALREADY DEFINED\n", attribute_name);
        return FALSE;
    }

    if (is_att_name(object_type, attribute_name))
    {
        printf(" DEFINITION ERROR - ATTRIBUTE NAME %s FOR %s ALREADY DEFINED\n", attribute_name, object_type);
        return FALSE;
    }

    temp = (obj_type_node *) obj_type_locate(object_types, object_type);

    if (temp)
    {
        list_insert_name(&(temp->att_list), attribute_name);
        list_insert_name(&attribute_names, attribute_name);
        return TRUE;
    }
    else
        return FALSE;
}

int add_rel_def(char *domain, char *relation, char *range, char *inverse)
{
    if (is_obj_type(relation))
    {
        printf(" DEFINITION ERROR - OBJECT TYPE %s ALREADY DEFINED\n", relation);
        return FALSE;
    }

    if (is_obj_type(inverse))
    {
        printf(" DEFINITION ERROR - OBJECT TYPE %s ALREADY DEFINED\n", inverse);
        return FALSE;
    }

    if (is_global_att_name(relation))
    {
        printf(" DEFINITION ERROR - ATTRIBUTE NAME %s ALREADY DEFINED\n", relation);
        return FALSE;
    }

    if (is_global_att_name(relation))
    {
        printf(" DEFINITION ERROR - ATTRIBUTE NAME %s ALREADY DEFINED\n", inverse);
        return FALSE;
    }

    if (!is_obj_type(domain))
    {
        printf(" DEFINITION ERROR - OBJECT TYPE %s NOT DEFINED\n", domain);
        return FALSE;
    }

    if (!is_obj_type(range))
    {
        printf(" DEFINITION ERROR - OBJECT TYPE %s NOT DEFINED\n", range);
        return FALSE;
    }

    if (is_rel_def(domain, relation, range))
    {
        printf(" DEFINITION ERROR - RELATION %s %s %s ALREADY DEFINED\n", domain, relation, range);
        return FALSE;
    }

    if (is_rel_def(range, inverse, domain))
    {
        printf(" DEFINITION ERROR - RELATION %s %s %s ALREADY DEFINED\n", range, inverse, domain);
        return FALSE;
    }

    list_insert_name(&relation_names, relation);
    list_insert_name(&relation_names, inverse);

    add_rel_name(domain, relation);
    add_rel_name(range, inverse);

    rel_def_insert(&relation_defs, domain, relation, range, inverse);
    rel_def_insert(&relation_defs, range, inverse, domain, relation);
    return TRUE;
}

int add_object(obj_name name, obj_type type)
{
    if (!is_obj_type(type))
    {
        printf(" DESCRIPTION ERROR - TYPE %s NOT DEFINED\n", type);
        return FALSE;
    }

/*	if (is_obj_name(name)) */
/*	{ */
/*		printf(" DESCRIPTION ERROR - OBJECT %s ALREADY DEFINED\n", name); */
/*		return FALSE; */
/*	} */

    if (is_global_rel_name(name))
    {
        printf(" DESCRIPTION ERROR - %s DEFINED AS RELATION NAME\n", type);
        return FALSE;
    }

    if (is_global_att_name(name))
    {
        printf(" DESCRIPTION ERROR - %s DEFINED AS ATTRIBUTE NAME\n", type);
        return FALSE;
    }

    obj_insert(Object_Table, name, type);
    return TRUE;
}

int add_attribute(obj_name name, att_name attribute, char *text)
{
    int i;

    if (!is_obj_name(name))
    {
        printf(" DESCRIPTION ERROR - OBJECT %s NOT DEFINED\n", name);
        return FALSE;
    }

    if (!is_att_name(find_type(name), attribute))
    {
        printf(" DESCRIPTION ERROR - ATTRIBUTE %s NOT DEFINED FOR %s\n", attribute, name);
        return FALSE;
    }

    i = att_search(Object_Table, attribute);
    list_insert_text(&(Object_Table[i]->attributes), attribute, text);
    return TRUE;
}

int add_relation(obj_name name, rel_name relation, obj_name obj_of_rel)
{
    int i;

#ifdef DEBUG_PARSER
    printf("%s %s %s\n", name, relation, obj_of_rel);
#endif

    if (!is_rel_name(find_type(name), relation))
    {
        printf(" DESCRIPTION ERROR - RELATION %s NOT DEFINED FOR %s\n", relation, name);
        return FALSE;
    }

    if (is_obj_type(obj_of_rel))
    {
        printf(" DESCRIPTION ERROR - %s DEFINED AS OBJECT TYPE\n", obj_of_rel);
        return FALSE;
    }

    if (is_global_rel_name(obj_of_rel))
    {
        printf(" DESCRIPTION ERROR - %s DEFINED AS RELATION NAME\n", obj_of_rel);
        return FALSE;
    }

    if (is_global_att_name(obj_of_rel))
    {
        printf(" DESCRIPTION ERROR - %s DEFINED AS ATTRIBUTE NAME\n", obj_of_rel);
        return FALSE;
    }

    i = rel_insert(Relation_Table, name, relation);
    list_insert_name(&(Relation_Table[i]->obj_list), obj_of_rel);
    return TRUE;
}

int update_relations()
{
    int i = 0, j, status = TRUE, size_obj, size_rel;
    list curr;
    char           *domain, *range, *relation;

    printf(" Updating relations\n");

    while (Relation_Table[i])
    {
        curr = Relation_Table[i]->obj_list;

        while (curr)
        {
            if (range = find_type(curr->name))
            {
                domain = find_type(Relation_Table[i]->subject);
                relation = find_inverse(domain, Relation_Table[i]->relation, range);
                status &= add_relation(curr->name, relation, Relation_Table[i]->subject);
            }
            else
            {
                list_insert_name(&undeclared, curr->name);
/*				list_delete(&(Relation_Table[i]->obj_list), curr->name); */
            }
            curr = curr->next;
        }
        i++;
    }

    size_obj = obj_size(Object_Table);
    size_rel = rel_size(Relation_Table);

    /*	need to work more on relations. */
    for (i = 0; i < size_obj; i++)
    {
        for (j = 0; j < size_rel; j++)
        {
            if (strcmp(Relation_Table[j]->subject,Object_Table[i]->name))
                continue;
            if (!strcmp(Relation_Table[j]->relation,"called_by") || !strcmp(Relation_Table[j]->relation,"used_by"))
                break;
        }
        if (j >= size_rel)
        {
            if (!strcmp(Object_Table[i]->type,"global"))
                list_insert_name(&unused_globals, Object_Table[i]->name);
            if (!strcmp(Object_Table[i]->type,"subroutine"))
                list_insert_name(&unused_subroutines, Object_Table[i]->name);
        }
    }

    printf(" Sorting objects\n");
    obj_sort_name(Object_Table, 0, obj_size(Object_Table) - 1);

    printf(" Sorting relations\n");
    rel_sort_object(Relation_Table, 0, rel_size(Relation_Table) - 1);

    printf("\n %d objects\n\n %d relations\n\n %d undeclared\n\n %d unused globals\n\n %d unused subroutines\n",
           obj_size(Object_Table),
           rel_size(Relation_Table),
           list_length(undeclared),
           list_length(unused_globals),
           list_length(unused_subroutines));

    return status;
}

void dispose_vocab()
{
    obj_type_dispose(&object_types);
    rel_def_dispose(&relation_defs);
    list_dispose(&attribute_names);
    list_dispose(&relation_names);
}

void dispose_desc()
{
    obj_dispose(Object_Table);
    rel_dispose(Relation_Table);
    list_dispose(&undeclared);
    list_dispose(&unused_globals);
    list_dispose(&unused_subroutines);
}

int is_empty_vocab()
{
    return (!object_types && !relation_defs);
}

int is_empty_desc()
{
    return (!Object_Table[0] && !Relation_Table[0]);
}
