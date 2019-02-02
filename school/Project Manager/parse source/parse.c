#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "console.h"
#include "parse.h"

int main(int argc, char **argv)
{
    int     i, n;
    FILE   *input, *output;

    char    curr, prev, buffer[64];
    int     brace, bracket, paren;
    int     com_flag, scom_flag, str_flag;
    
    type    kind, line;

    tree    subroutines = NULL;
    tree    globals = NULL;

    // argc = ccommand(&argv);

    output = fopen("parse.out", "w");

    for (n = 1; n < argc; n++)
    {
        i = brace = bracket = paren = 0;
        com_flag = scom_flag = str_flag = OUT;
        kind = none;
        line = inner;

        fprintf(output, "* %02d\t%s", n, argv[n]);
        input = fopen(argv[n], "r");

        while ((curr = getc(input)) != EOF)
        {
            switch (curr)
            {
            case '*':                   // start of comment
                if ((prev == '/') && str_flag)
                    com_flag = IN;
                break;

//          case '#':                   // skip define's
//                  scom_flag = IN;
//                  break;

            case '/':                   // end of comment
                if ((prev == '*') && str_flag)
                    com_flag = OUT;
                if ((prev == '/') && str_flag)
                    scom_flag = IN;
                break;

            case '"':                   // toggles str_flg when character is a '"'
                if ((prev != '\'') && (prev != '\\') && com_flag)
                    str_flag = !str_flag;
                break;

            case '\n':                  // reset str_flag */
                str_flag = OUT;
                scom_flag = OUT;
                break;

            case '?':                   // handle special character constant
                if (prev == '\'')
                    curr = ' ';
                break;

            case '{':                   // handle special character constant
                if (prev == '\'')
                    curr = ' ';
                break;

            case '}':                   // handle special character constant
                if (prev == '\'')
                    curr = ' ';
                break;
            }

            if (com_flag && scom_flag && str_flag)
            {
                if (isalnum(curr) || (curr == '_'))
                {
                    buffer[i++] = curr;
                }
                else
                {
                    buffer[i] = 0;

                    if (curr == '{')
                        brace++;
                    if (curr == '}')
                    {
                        brace--;
                        if (brace < 0)
                            brace = 0;
                    }
                    
                    if (i && !iskeyword(buffer) && isalpha(buffer[0]))
                    {
                        switch (curr)
                        {
                        case '(' :
                            kind = subroutine;
                            break;
                        case ';' :
                        case ',' :
                        case '[' :
                        case ')' :
//                      case '\n' :
//                      case '\t' :
//                      case ' ' :
                        case '=' :
                            kind = global;
                            break;
                        default:
                            kind = none;
                        }
                        
                        if (kind == subroutine && !brace && !paren)
                        {
                            fprintf(output, ".\n\n");

                            fprintf(output, "subroutine %s",buffer);
                            kind = none;
                            line = outer;
                        }
                        else if (kind == global && !brace && !paren)
                        {
                            fprintf(output, ".\n\n");

                            fprintf(output, "global %s",buffer);
                            insert(&globals, buffer);
                            kind = none;
                            line = outer;
                        }
                        else if (kind == subroutine && (brace || paren))
                        {
                            if (line == inner)
                                fprintf(output, ";\n");
                            else
                                fprintf(output, "\n");

                            fprintf(output, "    calls %s",buffer);
                            kind = none;
                            line = inner;
                        }
                        else if (kind == global && (brace || paren) && locate(globals, buffer))
                        {
                            if (line == inner)
                                fprintf(output, ";\n");
                            else
                                fprintf(output, "\n");

                            fprintf(output, "    uses %s",buffer);
                            kind = none;
                            line = inner;
                        }                       
                    }

                    if (curr == '(')
                        paren++;
                    if (curr == ')')
                    {
                        paren--;
                        if (paren < 0)
                            paren = 0;
                    }

                    i = 0;
                }
            }

            prev = curr;                // set previous character to current
        }
        fprintf(output, ".\n\n");
        fclose(input);
    }
    fclose(output);
    return 0;
}

int iskeyword(char target[])
{
    int     top, bottom, mid;
    int     from = 0, to = 30;
    char   *keywords[] =
    {
        "break",
        "case",
        "char",
        "const",
        "default",
        "define",
        "do",
        "double",
        "else",
        "enum",
        "extern",
        "float",
        "for",
        "if",
        "include",
        "inline",
        "int",
        "long",
        "register",
        "return",
        "short",
        "signed",
        "sizeof",
        "static",
        "struct",
        "switch",
        "typedef",
        "union",
        "unsigned",
        "void",
        "while"
    };

    bottom = from;
    top = to;
    mid = (top + bottom) / 2;

    while (bottom < top)
    {
        if (strcmp(target, keywords[mid]) <= 0)
            top = mid;
        else
            bottom = mid + 1;

        mid = (top + bottom) / 2;
    }

    if (!strcmp(target, keywords[mid]))
        return 1;
    else
        return 0;
}

void insert(tree *root, char name[])
{
    int     i;

    if (*root)
    {
        if (!(i = strcmp(name, (*root)->name)))
            return;

        else if (i < 0)
            insert(&((*root)->left), name);

        else if (i > 0)
            insert(&((*root)->right), name);
    }
    else
    {
        (*root) = (node *) malloc(sizeof(node));

        if (*root)
        {
            strcpy((*root)->name, name);
            (*root)->right = NULL;
            (*root)->left = NULL;
        }
        else
        {
            printf("FATAL ERROR - UNABLE TO ALLOCATE MEMORY\n");
        }
    }
}

tree locate(tree root, char name[])
{
    int     i;

    if (root)
    {
        if (!(i = strcmp(name, root->name)))
            return root;
        else if (i < 0)
            return locate(root->left, name);
        else if (i > 0)
            return locate(root->right, name);
    }
    return NULL;
}

void print(tree root)
{
    if (root)
    {
        if (root->left)
            print(root->left);
        if (root->name)
            printf("global %s;\n", root->name);
        if (root->right)
            print(root->right);
    }
}
