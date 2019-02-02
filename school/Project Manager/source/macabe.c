/* Module: macabe.c
 *
 * Purpose: This module count regions and lines from a	C program. Also the module counts the
 * occurences of "if", "else", "for", "case", "while", and "?" that occur outside of comments and
 * string constants. The algorithm is to assemble a word in a buffer made of legal characters,
 * i.e, alphabet:	A-Z, a-z; digits: 0-9; and underscoure '_'. The buffer is then compared
 * with key words.
 *
 * Written by Jesse Costales, Ismael Flores, Julian Escontrias, and Mike Marez.
 *
 * Input: C program
 *
 * Output: Number of regions and lines.
 *
 */

#include "project_manager.h"

extern FILE    *input;

void Macabe(int *line_count, int *region_count)
{
    char curr, prev;                    /*	current and previous character */
    char buffer[256];                   /*	buffer for string to compare */
    int com_flag = 1;                   /*	0 if in comment */
    int str_flag = 1;                   /*	0 if in string */
    int i = 0;

    while ((curr = getc(input)) != EOF)
    {
        /* determine if the character is in comment or string constant do not set or reset
         * str_flag if in comments and vicea-versa. Example " will not toggle str_flag because
         * it occurs in this comment. */
        switch (curr)
        {
        case '*':                       /*	start of comment */
            if ((prev == '/') && str_flag)
                com_flag = 0;
            break;
        case '/':                       /*	end of comment */
            if ((prev == '*') && str_flag)
                com_flag = 1;
            break;

        /*	Character constant '?', '"', \" should be ignored */

        case '"':                       /*	toggles str_flg when character is a '"' */
            if ((prev != '\'') && (prev != '\\') && com_flag)
                str_flag = !str_flag;
            break;

        case '?':                       /*	handle special character constant */
            if (prev == '\'')
                curr = ' ';
            break;

        case '\n':                      /*	reset str_flag */
            str_flag = 1;
            break;
        }

        if (com_flag && str_flag)
        {
            if (isalnum(curr) || (curr == '_') || (curr == '?'))
                buffer[i++] = curr;     /*	build key words */
            else if ((curr == '\n') && (prev != '\n'))
                (*line_count)++;        /*	count lines */
            else if (i)
            {
                /*	do comparisons and count regions & lines */

                buffer[i] = 0;
                if (strcmp(buffer,"if") == 0)
                    (*region_count)++;
                if (strcmp(buffer,"else") == 0)
                    (*region_count)++;
                if (strcmp(buffer,"for") == 0)
                    (*region_count)++;
                if (strcmp(buffer,"case") == 0)
                    (*region_count)++;
                if (strcmp(buffer,"while") == 0)
                    (*region_count)++;
                if (strcmp(buffer,"?") == 0)
                    (*region_count)++;
                buffer[0] = 0;
                i = 0;
            }
        }
        prev = curr;                    /*	set previous character to current */
    }
}
