/*
 * Recursive descent parser for integer expressions which may include
 * variables and function calls.
 */

#include "LittleC.h"

extern char *prog;  /* current location in source code */
extern char *p_buf; /* points to start of program buffer */

extern variable_type global_vars[NUM_GLOBAL_VARS];
extern func_type func_stack[NUM_FUNC]; /* Funtion call stack */
extern commands table[];

int call_getche(void);
int call_putch(void);
int call_puts(void);
int getnum(void);
int print(void);

/* Internal Functions */
struct intern_func_type {
    char *f_name;                       /* function name */
    int (*p)();                         /* pointer to the function */
}

intern_func[] = {
    "getche", call_getche,
    "putch",  call_putch,
    "puts",   call_puts,
    "getnum", getnum,
    "print",  print,
    "", 0                               /* null terminate the list */
};

extern char token[MAX];                 /* string representation of token */
extern char token_type;                 /* contains type of token */
extern char tok;                        /* internal representation of token */
extern int ret_value;                   /* function return value */

/* Entry point into parser. */
void eval_exp(int *value)
{
    get_token();
    if (!*token) {
        sntx_err(NO_EXP);
        return;
    }
    if (*token == ';') {
        *value = 0;                     /* empty expression */
        return;
    }
    eval_exp0(value);
    put_back();                          /* put back last token read */
}

/* Process an assignment expression */
void eval_exp0(int *value)
{
    register int temp_tok;
    char temp[ID_LEN];                  /* holds name of assigned var */

    if (token_type == IDENTIFIER) {
        if (is_var(token)) {            /* if a var, see if assignment */
            strcpy(temp, token);
            temp_tok = token_type;
            get_token();
            if (*token == '=') {        /* is an assignment */
                get_token();
                eval_exp0(value);         /* get value to assign */
                assign_var(temp, *value); /* assign the value */
                return;
            } else {                    /* not an assignment */
                put_back();             /* restore original token */
                strcpy(token, temp);
                token_type = temp_tok;
            }
        }
    }
    eval_exp1(value);
}

/* This array is used by eval_exp1().  Because some compilers cannot */
/* initialize an array within a function it is defined as a global variable. */
char relops[] = {
    LT, LE, GT, GE, EQ, NE, 0
};

/* Process relational operators. */
void eval_exp1(int *value)
{
    register char op;
    int partial_value;

    eval_exp2(value);
    op = *token;
    if (strchr(relops, op)) {
        get_token();
        eval_exp2(&partial_value);
        switch (op) {                   /* perform the relational operation */
        case LT:
            *value = *value < partial_value;
            break;
        case LE:
            *value = *value <= partial_value;
            break;
        case GT:
            *value = *value > partial_value;
            break;
        case GE:
            *value = *value >= partial_value;
            break;
        case EQ:
            *value = *value == partial_value;
            break;
        case NE:
            *value = *value != partial_value;
            break;
        }
    }
}

/* Add or subtract two terms. */
void eval_exp2(int *value)
{
    register char op;
    int partial_value;

    eval_exp3(value);
    while ((op = *token) == '+' || op == '-') {
        get_token();
        eval_exp3(&partial_value);
        switch (op) {                   /* add or subtract */
        case '-':
            *value = *value - partial_value;
            break;
        case '+':
            *value = *value + partial_value;
            break;
        }
    }
}

/* Multiply or divide two factors. */
void eval_exp3(int *value)
{
    register char op;
    int partial_value, t;

    eval_exp4(value);
    while ((op = *token) == '*' || op == '/' || op == '%') {
        get_token();
        eval_exp4(&partial_value);
        switch (op) {                   /* mul, div, or modulus */
        case '*':
            *value = (*value) * partial_value;
            break;
        case '/':
            *value = (*value) / partial_value;
            break;
        case '%':
            t      = (*value) / partial_value;
            *value = (*value) - (t * partial_value);
            break;
        }
    }
}

/* Is a unary + or -. */
void eval_exp4(int *value)
{
    register char op = '\0';

    if (*token == '+' || *token == '-') {
        op = *token;
        get_token();
    }
    eval_exp5(value);
    if (op) {
        if (op == '-')
            *value = -(*value);
    }
}

/* Process parenthesized expression. */
void eval_exp5(int *value)
{
    if (*token == '(') {
        get_token();
        eval_exp0(value);               /* get subexpression */
        if (*token != ')')
            sntx_err(PAREN_EXPECTED);
        get_token();
    } else {
        atom(value);
    }
}

/* Find value of number, variable or function. */
void atom(int *value)
{
    int i;
    switch (token_type) {
    case IDENTIFIER:
        i = internal_func(token);
        if (i != -1) {                  /* call "standard library" function */
            *value = (*intern_func[i].p)();
        } else if (find_func(token)) {  /* call user-defined function */
            call();
            *value = ret_value;
        } else {
            *value = find_var(token);   /* get var's value */
        }
        get_token();
        return;

    case NUMBER:                        /* is numeric constant */
        *value = atoi(token);
        get_token();
        return;

    case DELIMITER:                     /* see if character constant */
        if (*token == '\'') {
            *value = *prog;
            prog++;
            if (*prog != '\'')
                sntx_err(QUOTE_EXPECTED);
            prog++;
            get_token();
        }
        return;

    default:
        if (*token == ')')
            return;                     /* process empty expression */
        else
            sntx_err(SYNTAX);           /* syntax error */
    }
} /* atom */

/* Display an error message. */
void sntx_err(int error)
{
    char *p, *temp;
    int linecount = 0;
    register int i;
    static char *e[] = {
        "syntax error",
        "unbalanced parentheses",
        "no expression present",
        "equals sign expected",
        "not a variable",
        "parameter error",
        "semicolon expected",
        "unbalanced braces",
        "function undefined",
        "type specifier expected",
        "too many nested function calls",
        "return without call",
        "parentheses expected",
        "while expected",
        "closing quote expected",
        "not a string",
        "too many local variables"
    };

    printf("%s", e[error]);
    p = p_buf;
    while (p != prog) {                 /* find line number of error */
        p++;
        if (*p == '\n')
            linecount++;
    }
    printf(" in line %d\n", linecount);
    temp = p;
    for (i = 0; i < 20 && p > p_buf && *p != '\n'; i++, p--)
        ;
    for (i = 0; i < 30 && p <= temp; i++, p++)
        printf("%c", *p);
} /* sntx_err */

/* Get a token. */
int get_token(void)
{
    register char *temp;

    token_type = 0;
    tok        = 0;
    temp       = token;
    *temp      = '\0';

    /* skip over white space */
    while (iswhite(*prog) && *prog)
        ++prog;

    if (*prog == '\n') {
        ++prog;
        /* skip over white space */
        while (iswhite(*prog) && *prog)
            ++prog;
    }
    if (*prog == '\0') {                /* end of file */
        *token = '\0';
        tok    = FINISHED;
        return (token_type = DELIMITER);
    }
    if (strchr("{}", *prog)) {          /* block delimiters */
        *temp = *prog;
        temp++;
        *temp = '\0';
        prog++;
        return (token_type = BLOCK);
    }
    /* look for comments */
    if (*prog == '/') {
        if (*(prog + 1) == '*') {       /* is a comment */
            prog += 2;
            do {                        /* find end of comment */
                while (*prog != '*')
                    prog++;
                prog++;
            } while (*prog != '/');
            prog++;
        }
    }
    if (strchr("!<>=", *prog)) {        /* is or might be a relation operator */
        switch (*prog) {
        case '=':
            if (*(prog + 1) == '=') {
                prog++;
                prog++;
                *temp = EQ;
                temp++;
                *temp = EQ;
                temp++;
                *temp = '\0';
            }
            break;
        case '!':
            if (*(prog + 1) == '=') {
                prog++;
                prog++;
                *temp = NE;
                temp++;
                *temp = NE;
                temp++;
                *temp = '\0';
            }
            break;
        case '<':
            if (*(prog + 1) == '=') {
                prog++;
                prog++;
                *temp = LE;
                temp++;
                *temp = LE;
            } else {
                prog++;
                *temp = LT;
            }
            temp++;
            *temp = '\0';
            break;
        case '>':
            if (*(prog + 1) == '=') {
                prog++;
                prog++;
                *temp = GE;
                temp++;
                *temp = GE;
            } else {
                prog++;
                *temp = GT;
            }
            temp++;
            *temp = '\0';
            break;
        }
        if (*token)
            return (token_type = DELIMITER);
    }
    if (strchr("+-*^/%=;(),'", *prog)) { /* delimiter */
        *temp = *prog;
        prog++;                         /* advance to next position */
        temp++;
        *temp = '\0';
        return (token_type = DELIMITER);
    }
    if (*prog == '"') {                 /* quoted string */
        prog++;
        while (*prog != '"' && *prog != '\n')
            *temp++ = *prog++;
        if (*prog == '\n')
            sntx_err(SYNTAX);
        prog++;
        *temp = '\0';
        return (token_type = QUOTE);
    }
    if (isdigit(*prog)) {               /* number */
        while (!isdelim(*prog))
            *temp++ = *prog++;
        *temp = '\0';
        return (token_type = NUMBER);
    }
    if (isalpha(*prog)) {               /* var or command */
        while (!isdelim(*prog))
            *temp++ = *prog++;
        token_type = TEMP;
    }
    *temp = '\0';
    /* see if a string is a command or a variable */
    if (token_type == TEMP) {
        tok = look_up(token);           /* convert to internal rep */
        if (tok)
            token_type = KEYWORD;       /* is a keyword */
        else
            token_type = IDENTIFIER;
    }
    return token_type;
} /* get_token */

/* Return a token to input stream. */
void put_back(void)
{
    char *t;

    t = token;
    for (; *t; t++)
        prog--;
}

/* Look up a token's internal representation in the token table. */
int look_up(char *s)
{
    char *p;

    /* convert to lowercase */
    p = s;
    while (*p) {
        *p = tolower(*p);
        p++;
    }
    /* see if token is in table */
    for (int i = 0; *table[i].command; i++)
        if (!strcmp(table[i].command, s))
            return table[i].tok;

    return 0; /* unknown command */
}

/* Return index of internal library function or -1 if not found. */
int internal_func(char *s)
{
    for (int i = 0; intern_func[i].f_name[0]; i++) {
        if (!strcmp(intern_func[i].f_name, s))
            return i;
    }
    return -1;
}

/* Return true if c is a delimiter. */
int isdelim(char c)
{
    if (strchr(" !;,+-<>'/*%^=()", c) ||
        c == '\t' || c == '\r' || c == '\n' || c == 0)
        return 1;
    return 0;
}

/* Return 1 if c is space or tab. */
int iswhite(char c)
{
    return (c == ' ' || c == '\t') ? 1 : 0;
}
