/*	Header File: data_manager.h */

#ifndef _data_manager
#define _data_manager

enum token
{
    END, COMMA, SEMICOLON, PERIOD, SLASH, DASH, STRING, IDENT, ERROR
};

typedef char identifier[32];
typedef identifier att_name, obj_name, obj_type, rel_name;

typedef int    *matrix;

typedef struct list_node
{
    identifier name;
    char           *text;
    struct list_node *next;
} list_node, *list;

typedef struct obj_type_node
{
    obj_type type;
    list att_list, rel_list;
    struct obj_type_node *left, *right;
} obj_type_node, *ot_tree;

typedef struct rel_def_node
{
    obj_type domain;
    rel_name relation;
    obj_type range;
    rel_name inverse;
    struct rel_def_node *left, *right;
} rel_def_node, *rd_tree;

typedef struct obj_node
{
    obj_type type;
    obj_name name;
    list attributes;
} obj_node, *object;

typedef struct rel_node
{
    obj_name subject;
    rel_name relation;
    list obj_list;
} rel_node, *relationship;

#endif
