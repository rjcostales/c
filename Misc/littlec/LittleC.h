#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#ifndef _LC_H_
#define _LC_H_

#define NUM_FUNC        100
#define NUM_GLOBAL_VARS 100
#define NUM_LOCAL_VARS  200
#define NUM_BLOCK       100
#define ID_LEN          31
#define FUNC_CALLS      31
#define NUM_PARAMS      31
#define PROG_SIZE       10000
#define LOOP_NEST       31

enum tok_types {
  DELIMITER, IDENTIFIER, NUMBER, KEYWORD, TEMP, BLOCK, QUOTE
};

/* add additional C keyword tokens here */
enum tokens {
  ARG, CHAR, INT, IF, ELSE, FOR, DO, WHILE, SWITCH, RETURN, EOL,
  FINISHED, END
};

/* add additional double operators here (such as ->) */
enum double_ops {
  LT = 1, LE, GT, GE, EQ, NE
};

/* These are the constants used to call sntx_err() when a syntax error occurs.
 * Add more if you like.
 * NOTE: SYNTAX is a generic error message used when nothing else seems 
 * appropriate.
 */
enum error_msg {
  SYNTAX, UNBAL_PARENS, NO_EXP, EQUALS_EXPECTED, NOT_VAR, PARAM_ERR,
  SEMI_EXPECTED, UNBAL_BRACES, FUNC_UNDEF, TYPE_EXPECTED, NEST_FUNC,
  RET_NOCALL, PAREN_EXPECTED, WHILE_EXPECTED, QUOTE_EXPECTED, NOT_TEMP,
  TOO_MANY_LVARS
};

/* An array of these structures holds the info for global variables. */
typedef struct variable_type {
  char var_name[ID_LEN];
  int var_type;
  int value;
} variable_type;

typedef struct func_type {
  char func_name[ID_LEN];
  char *loc;                    /* location of entry point in file */
} func_type;

typedef struct commands {       /* keyword lookup table */
  char command[20];
  char tok;
} commands;

/* LittleC.c */
int main (int, char *[]);
void interp_block (void);
int load_program (char *, char *);
void prescan (void);
char *find_func (char *);
void decl_global (void);
void decl_local (void);
void call (void);
void get_args (void);
void get_params (void);
void func_ret (void);
void local_push (struct variable_type);
int func_pop (void);
void func_push (int);
void assign_var (char *, int);
int find_var (char *);
int is_var (char *);
void exec_if (void);
void exec_while (void);
void exec_do (void);
void find_eob (void);
void exec_for (void);

/* Parser.c */
void eval_exp (int *);
void eval_exp0 (int *);
void eval_exp1 (int *);
void eval_exp2 (int *);
void eval_exp3 (int *);
void eval_exp4 (int *);
void eval_exp5 (int *);
void atom (int *);
void sntx_err (int);
int get_token (void);
void putback (void);
int look_up (char *);
int internal_func (char *);
int isdelim (char);
int iswhite (char);

#endif
