/*
 * A Little C interpreter.
 *
 * Herbert Schildt,"Building your own C Interpreter."
 * Dr. Dobb's Journal of Software Tools v14, (August, 1989):38 (16 pages).
 */

#include "LittleC.h"

func_type func_table[NUM_FUNC];

char *prog;             /* current location in source code */
char *p_buf;            /* points to start of program buffer */

int call_stack[NUM_FUNC];
variable_type global_vars[NUM_GLOBAL_VARS];
variable_type local_var_stack[NUM_LOCAL_VARS];

commands table[] = {    /* Commands must be entered lowercase */
    "if",     IF,       /* in this table. */
    "else",   ELSE,
    "for",    FOR,
    "do",     DO,
    "while",  WHILE,
    "char",   CHAR,
    "int",    INT,
    "return", RETURN,
    "end",    END,
    "",       END       /* mark end of table */
};

char token[MAX];
char token_type, tok;

int functos;            /* index to top of function call stack */
int func_index;         /* index into function table */
int gvar_index;         /* index into global variable table */
int lvartos;            /* index into local variable stack */
int ret_value;          /* function return value */

int main(int argc, char *argv[])
{
    printf("Little C Interpreter\n\n");

    if (argc != 2) {
        printf("usage: LittleC <filename>\n");
        exit(1);
    }
    /* allocate memory for the program */
    if ((p_buf = (char *) calloc(PROG_SIZE, sizeof(char))) == NULL) {
        printf("Program buffer allocation failure.");
        exit(1);
    }
    prog = p_buf;                       /* set prog ptr to start of buffer */
    if (!load_program(p_buf, argv[1]))  /* load the program to execute */
        exit(1);

    /* if (setjmp(e_buf)) exit(1); */
    /* initialize long jump buffer */
    prescan();                          /* find the all functions and global
                                         * variables in the program */
    gvar_index = 0;                     /* init global variable index */
    lvartos    = 0;                     /* init local variable stack index */
    functos    = 0;                     /* init the CALL stack index */
    /* setup call to main() */
    prog = find_func("main");           /* find program starting point */
    prog--;                             /* back up to opening ( */
    strcpy(token, "main");
    call();                             /* call main() to start interpreting */
    return 0;
}

/*
 * Interpret a single statement or block of code.  When interp_block()
 * returns from it's initial call, the final brace (or a return) in main()
 * has been encountered.
 */
void interp_block(void)
{
    int value;
    char block = 0;

    do {
        token_type = get_token();

        /*
         * If interpreting single statement,
         * return on first semicolon.
         */
        /* see what kind of token is up */
        if (token_type == IDENTIFIER) {
            /* Not a keyword, so process expression. */
            put_back();                 /* restore token to input stream for
                                         * further processing by eval_exp() */
            eval_exp(&value);           /* process the expression */
            if (*token != ';')
                sntx_err(SEMI_EXPECTED);
        } else if (token_type == BLOCK) { /* if block delimiter */
            if (*token == '{')          /* is a block */
                block = 1;              /* interpreting block, not statement */
            else
                return;                 /* is a }, so return */
        } else {                        /* is keyword */
            switch (tok) {
                case CHAR:
                case INT:               /* declare local variables */
                    put_back();
                    decl_local();
                    break;
                case RETURN:            /* return from function call */
                    func_ret();
                    return;

                case IF:                /* process an if statement */
                    exec_if();
                    break;
                case ELSE:              /* process an else statement */
                    find_eob();         /* find end of else block & continue */
                    break;
                case WHILE:             /* process a while loop */
                    exec_while();
                    break;
                case DO:                /* process a do-while loop */
                    exec_do();
                    break;
                case FOR:
                    exec_for();
                    break;
                case END:
                    exit(0);
            }
        }
    } while (tok != FINISHED && block);
} /* interp_block */

/* Load a program. */
int load_program(char *p, char *fname)
{
    FILE * fp;
    int i = 0, c;

    if ((fp = fopen(fname, "r")) == NULL)
        return 0;

    while ((c = getc(fp)) != EOF && i < PROG_SIZE) {
        *p = c;
        p++;
        i++;
    }
    *p = '\0';                          /* null terminate the program */
    fclose(fp);
    return 1;
}

/* Find the loc of all functions in the program and store global variables. */
void prescan(void)
{
    char *p;
    char temp[32];
    int brace = 0;                      /* When 0, this var tells us that
                                         * current source position is outside
                                         * of any function. */

    p = prog;
    func_index = 0;
    do {
        while (brace) {                 /* bypass code inside functions */
            get_token();
            if (*token == '{')
                brace++;
            if (*token == '}')
                brace--;
        }
        get_token();
        if (tok == CHAR || tok == INT) { /* is global var */
            put_back();
            decl_global();
        } else if (token_type == IDENTIFIER) {
            strcpy(temp, token);
            get_token();
            if (*token == '(') {        /* must be assume a function */
                func_table[func_index].loc = prog;
                strcpy(func_table[func_index].func_name, temp);
                func_index++;
                while (*prog != ')')
                    prog++;
                prog++;

            /* prog points to opening curly brace of function */
            } else {
                put_back();
            }
        } else if (*token == '{') {
            brace++;
        }
    } while (tok != FINISHED);
    prog = p;
} /* prescan */

/*
 * Return the entry point of the specified function.
 * Return NULL if not found.
 */
char *find_func(char *name)
{
    for (int i = 0; i < func_index; i++)
        if (!strcmp(name, func_table[i].func_name))
            return func_table[i].loc;
    return NULL;
}

/* Declare a global variable. */
void decl_global(void)
{
    get_token(); /* get type */
    global_vars[gvar_index].var_type = tok;
    global_vars[gvar_index].value    = 0; /* init to 0 */
    do {                                  /* process comma-separated list */
        get_token();                      /* get name */
        strcpy(global_vars[gvar_index].var_name, token);
        get_token();
        gvar_index++;
    } while (*token == ',');
    if (*token != ';')
        sntx_err(SEMI_EXPECTED);
}

/* Declare a local variable. */
void decl_local(void)
{
    variable_type i;

    get_token(); /* get type */
    i.var_type = tok;
    i.value    = 0;                     /* init to 0 */
    do {                                /* process comma-separated list */
        get_token();                    /* get var name */
        strcpy(i.var_name, token);
        local_push(i);
        get_token();
    } while (*token == ',');
    if (*token != ';')
        sntx_err(SEMI_EXPECTED);
}

/* Call a function. */
void call(void)
{
    char *loc, *temp;
    int lvartemp;

    loc = find_func(token);             /* find entry point of function */
    if (loc == NULL) {
        sntx_err(FUNC_UNDEF);           /* function not defined */
    } else {
        lvartemp = lvartos;             /* save local var stack index */
        get_args();                     /* get function arguments */
        temp = prog;                    /* save return location */
        func_push(lvartemp);            /* save local var stack index */
        prog = loc;                     /* reset prog to start of function */
        get_params();                   /* load the function's parameters
                                         * with the values of the arguments */
        interp_block();                 /* interpret the function */
        prog    = temp;                 /* reset the program pointer */
        lvartos = func_pop();           /* reset the local var stack */
    }
}

/* Push the arguments to a function onto the local variable stack. */
void get_args(void)
{
    int value, count, temp[NUM_PARAMS];
    variable_type i;

    count = 0;
    get_token();
    if (*token != '(')
        sntx_err(PAREN_EXPECTED);
    /* process a comma-separated list of values */
    do {
        eval_exp(&value);
        temp[count] = value;            /* save temporarily */
        get_token();
        count++;
    } while (*token == ',');
    count--;
    /* now, push on local_var_stack in reverse order */
    for (; count >= 0; count--) {
        i.value    = temp[count];
        i.var_type = ARG;
        local_push(i);
    }
}

/* Get function parameters. */
void get_params(void)
{
    variable_type * p;
    int i;

    i = lvartos - 1;
    do {                                /* process comma-separated parameters */
        get_token();
        p = &local_var_stack[i];
        if (*token != ')') {
            if (tok != INT && tok != CHAR)
                sntx_err(TYPE_EXPECTED);
            p->var_type = token_type;
            get_token();

            /* link parameter name with argument already on local var stack */
            strcpy(p->var_name, token);
            get_token();
            i--;
        } else {
            break;
        }
    } while (*token == ',');
    if (*token != ')')
        sntx_err(PAREN_EXPECTED);
}

/* Return from a function. */
void func_ret(void)
{
    int value;

    value = 0;
    /* get return value, if any */
    eval_exp(&value);
    ret_value = value;
}

/* Push local variable */
void local_push(variable_type i)
{
    if (lvartos > NUM_LOCAL_VARS)
        sntx_err(TOO_MANY_LVARS);
    local_var_stack[lvartos] = i;
    lvartos++;
}

/* Pop index into local variable stack. */
int func_pop(void)
{
    functos--;
    if (functos < 0)
        sntx_err(RET_NOCALL);
    return (call_stack[functos]);
}

/* Push index of local variable stack. */
void func_push(int i)
{
    if (functos > NUM_FUNC)
        sntx_err(NEST_FUNC);
    call_stack[functos] = i;
    functos++;
}

/* Assign a value to a variable. */
void assign_var(char *var_name, int value)
{
    register int i;

    /* first, see if it's a local variable */
    for (i = lvartos - 1; i >= call_stack[functos - 1]; i--) {
        if (!strcmp(local_var_stack[i].var_name, var_name)) {
            local_var_stack[i].value = value;
            return;
        }
    }
    if (i < call_stack[functos - 1]) {
        /* if not local, try global var table */
        for (i = 0; i < NUM_GLOBAL_VARS; i++)
            if (!strcmp(global_vars[i].var_name, var_name)) {
                global_vars[i].value = value;
                return;
            }
    }
    sntx_err(NOT_VAR);                  /* variable not found */
}

/* Find the value of a variable. */
int find_var(char *s)
{
    /* first, see if it's a local variable */
    for (int i = lvartos - 1; i >= call_stack[functos - 1]; i--)
        if (!strcmp(local_var_stack[i].var_name, token))
            return local_var_stack[i].value;

    /* otherwise, try global vars */
    for (int i = 0; i < NUM_GLOBAL_VARS; i++)
        if (!strcmp(global_vars[i].var_name, s))
            return global_vars[i].value;

    sntx_err(NOT_VAR);                  /* variable not found */
    return 0;
}

/*
 * Determine if an identifier is a variable.
 * Return 1 if variable is found; 0 otherwise.
 */
int is_var(char *s)
{
    /* first, see if it's a local variable */
    for (int i = lvartos - 1; i >= call_stack[functos - 1]; i--)
        if (!strcmp(local_var_stack[i].var_name, token))
            return 1;

    /* otherwise, try global vars */
    for (int i = 0; i < NUM_GLOBAL_VARS; i++)
        if (!strcmp(global_vars[i].var_name, s))
            return 1;

    return 0;
}

/* Execute an IF statement. */
void exec_if(void)
{
    int cond;

    eval_exp(&cond);                    /* get left expression */
    if (cond) {                         /* is true so process target of IF */
        interp_block();
    } else {
        /* otherwise skip around IF block and process the ELSE, if present */
        find_eob();                     /* find start of next line */
        get_token();
        if (tok != ELSE) {
            put_back();                  /* restore token if no ELSE present */
            return;
        }
        interp_block();
    }
}

/* Execute a while loop. */
void exec_while(void)
{
    int cond;
    char *temp;

    put_back();
    temp = prog;                        /* save location of top of while loop */
    get_token();
    eval_exp(&cond);                    /* check the conditional expression */
    if (cond) {
        interp_block();                 /* if true, interpret */
    } else {                            /* otherwise, skip around loop */
        find_eob();
        return;
    }
    prog = temp;                        /* loop back to top */
}

/* Execute a do loop. */
void exec_do(void)
{
    int cond;
    char *temp;

    put_back();
    temp = prog;                        /* save location of top of do loop */
    get_token();                        /* get start of loop */
    interp_block();                     /* interpret loop */
    get_token();
    if (tok != WHILE)
        sntx_err(WHILE_EXPECTED);
    eval_exp(&cond);                    /* check the loop condition */
    if (cond)
        prog = temp;                    /* if true, loop;
                                         * otherwise, continue on */
}

/* Find the end of a block. */
void find_eob(void)
{
    int brace;

    get_token();
    brace = 1;
    do {
        get_token();
        if (*token == '{')
            brace++;
        else if (*token == '}')
            brace--;
    } while (brace);
}

/* Execute a while loop. */
void exec_for(void)
{
    int cond;
    char *temp, *temp2;
    int brace;

    get_token();
    eval_exp(&cond);                    /* initialization expression */
    if (*token != ';')
        sntx_err(SEMI_EXPECTED);
    prog++;                             /* get past the ; */
    temp = prog;
    for (;;) {
        eval_exp(&cond);                /* check the condition */
        if (*token != ';')
            sntx_err(SEMI_EXPECTED);
        prog++;                         /* get past the ; */
        temp2 = prog;
        /* find the start of the for block */
        brace = 1;
        while (brace) {
            get_token();
            if (*token == '(')
                brace++;
            if (*token == ')')
                brace--;
        }
        if (cond) {
            interp_block();             /* if true, interpret */
        } else {                        /* otherwise, skip around loop */
            find_eob();
            return;
        }
        prog = temp2;
        eval_exp(&cond);                /* do the increment */
        prog = temp;                    /* loop back to top */
    }
} /* exec_for */
