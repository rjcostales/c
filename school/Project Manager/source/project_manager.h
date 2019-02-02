/* Header File: project_manager.h */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "data_manager.h"

#ifndef _project_manager
#define _project_manager

#define MAX_OBJECTS     640
#define MAX_RELATIONS   640

#define BUFFER_SIZE     64

#ifndef __MWERKS__
#define TRUE            1
#define FALSE           0
#endif

#endif

void        wait();
void        InputDesc();
void        InputVocab();
void        MenuTitle();
void        MenuMain();
void        MenuMetrics();
void        MenuPrint();

int         is_obj_type(char *);
int         is_rel_name(char *, char *);
int         is_att_name(char *, char *);
int         is_global_att_name(char *);
int         is_global_rel_name(char *);
int         is_rel_def(char *, char *, char *);
int         is_obj_name(char *);
char       *find_type(obj_name);
char       *find_inverse(char *, char *, char *);
int         add_obj_type(char *);
int         add_rel_name(char *, char *);
int         add_att_name(char *, char *);
int         add_rel_def(char *, char *, char *, char *);
int add_object(obj_name, obj_type);
int add_attribute(obj_name, att_name, char *);
int add_relation(obj_name, rel_name, obj_name);
int         update_relations();
void        dispose_vocab();
void        dispose_desc();
int         is_empty_vocab();
int         is_empty_desc();

int         is_ident_char(char);
int         get_token();
int         skip_to_period();
int         skip_to_semicolon();
void        parse_error(char *);
int         parse_vocab(char *);
int         parse_desc(char *);

int         list_insert_name(list *, char *);
int         list_insert_text(list *, char *, char *);
char       *list_get_name(list, int);
char       *list_get_text(list, char *);
void        list_delete(list *, char *);
list list_locate(list, char *);
int list_index(list, char *);
void        list_dispose(list *);
int list_length(list);
int         obj_type_insert(ot_tree *, char *);
ot_tree     obj_type_locate(ot_tree root, char *);
void        obj_type_dispose(ot_tree *);
int         rel_def_insert(rd_tree *, char *, char *, char *, char *);
rd_tree     rel_def_locate(rd_tree root, char *, char *, char *);
void        rel_def_dispose(rd_tree *);

void        Macabe(int *, int *);

void list_print_name(list);
void list_print_text(list);
void obj_type_print(ot_tree);
void obj_type_count_print(ot_tree);
void rel_print_def(rd_tree);
int rel_print_list(obj_name);
void        rel_print_level(char *, int, int, int);
void        report_relations(char *);
void        report_obj_by_name(char *);
void        report_obj_by_type(char *);
void        report_vocab(char *);
void        report_desc(char *);
void        report_levels(char *);

matrix      matrix_new(int);
void matrix_clear(matrix, int);
void matrix_set(int, int, matrix, int);
void matrix_copy(matrix, matrix, int);
void matrix_product(matrix, matrix, matrix, int);
int matrix_circuit(matrix, int);
int matrix_zero(matrix, int);
void matrix_print(matrix, int);
void compute_levels(obj_type, rel_name);
void compute_calls(obj_type, rel_name);
void compute_complexity(obj_type, att_name);

int obj_insert(object *, obj_name, obj_type);
int         obj_size(object *);
int         obj_count_type(object *, char *);
void        obj_swap(object *, int, int);
void        obj_sort_name(object *, int, int);
void        obj_sort_type(object *, int, int);
void        obj_dispose(object *);
int rel_insert(relationship *, obj_name, rel_name);
int         rel_size(relationship *);
void        rel_swap(relationship *, int, int);
void        rel_sort_relation(relationship *, int, int);
void        rel_sort_object(relationship *, int, int);
void        rel_dispose(relationship *);
int att_search(object *, att_name);
