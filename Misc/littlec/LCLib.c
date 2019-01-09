/****** Internal Library Functions *******/

/* Add more of your own, here. */

#include "LittleC.h"

extern char *prog;              /* points to current location in program */
extern char token[80];          /* holds string representation of token */
extern char token_type;         /* contains type of token */
extern char tok;                /* holds the internal representation of token */

/* Use getchar()) if your compiler does not support getche() */
int call_getche(void)
{
    char ch;

    ch = getchar();

    while (*prog != ')')
        prog++;
    prog++;                     /* advance to end of line */
    return ch;
}

/* Use putchar() if your compiler does not support putch() */
int call_putch(void)
{
    int value;

    eval_exp(&value);
    printf("%c", value);
    return value;
}

/* Call puts(). */
int call_puts(void)
{
    get_token();

    if (*token != '(')
        sntx_err(PAREN_EXPECTED);

    get_token();
    if (token_type != QUOTE)
        sntx_err(QUOTE_EXPECTED);
    puts(token);

    get_token();
    if (*token != ')')
        sntx_err(PAREN_EXPECTED);

    get_token();
    if (*token != ';')
        sntx_err(SEMI_EXPECTED);

    putback();
    return 0;
}

/* Read an integer from the keyboard. */
int getnum(void)
{
    char s[80];

    gets(s);
    while (*prog != ')')
        prog++;
    prog++;                     /* advance to end of line */
    return atoi(s);
}

/* A built-in console output function. */
int print(void)
{
    int i;

    get_token();

    if (*token != '(')
        sntx_err(PAREN_EXPECTED);

    get_token();

    if (token_type == QUOTE)    /* output a string */
        printf("%s\n", token);
    else                        /* output a number */
    {
        putback();
        eval_exp(&i);
        printf("%d\n", i);
    }

    get_token();
    if (*token != ')')
        sntx_err(PAREN_EXPECTED);

    get_token();
    if (*token != ';')
        sntx_err(SEMI_EXPECTED);
    putback();
    return 0;
}
