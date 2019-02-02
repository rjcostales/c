/* Module: file_parser.c
 *
 * Purpose: The purpose of this module is to parse the vocabulary and description files and check
 * for any syntax errors. The two main functions of the whole module is parse_vocab() and
 * parse_desc() and they are called by the controller module.
 *
 * Written by Jesse Costales, Ismael Flores, Julian Escontrias, and Mike Marez.
 *
 * Note: The file_parser module calls the data_manager's update routines in order to add data to
 * the databases.
 *
 */

#include <stdio.h>
#include <ctype.h>

#include "project_manager.h"
#include "data_manager.h"

FILE           *input;                  /*	vocabulary or description */
char           *filename;               /*	used by parse_error to print file name */
int linecount = 1;                      /*	incremented by get_token and used by parse_error */
char buffer[BUFFER_SIZE];               /*	used by get_token to pass identifier or */
                                        /*	text to parse_vocab or parse_desc */

extern list attribute_names;
extern list relation_names;
extern list undeclared;

extern ot_tree object_types;
extern rd_tree relation_defs;

extern object Object_Table[];
extern relationship Relation_Table[];

/* UTILITY FUNCTIONS */

int is_ident_char(char c)
{
    return (isalnum(c) || (c == '_'));
}

/* returns token, also puts identifier into buffer */
int get_token()
{
    char c;
    int i;

    for (i = 0; i < BUFFER_SIZE; i++)   /* clear buffer */
        buffer[i] = 0;

    while (isspace(c = getc(input)))    /* skip spaces */
        if (c == '\n')                  /* count lines */
            linecount++;

    switch ((int) c)
    {
    case EOF:
        return END;
    case ',':
        return COMMA;
    case ';':
        return SEMICOLON;
    case '.':
        return PERIOD;
    case '/':
        return SLASH;
    case '-':
        return DASH;
    case '"':                           /* text returned in buffer */
    {
        int i = 0;

        while ((c = getc(input)) != '"')        /* look for end quote */
        {
            if (isprint(c))
            {
                if (i < BUFFER_SIZE)    /* bound check */
                {
                    buffer[i++] = c;
                }
                else
                {
                    while (getc(input) != '\n');        /* read to end of line */
                    linecount++;
                    parse_error("STRING TO LONG");
                    return ERROR;
                }
            }
            else
            {
                while (getc(input) != '\n');            /* read to end of line */
                linecount++;
                parse_error("ILLEGAL CHARACTER");
                return ERROR;
            }
        }
        return STRING;
    }
    case '*':                           /* COMMENTS */
        while ((c = getc(input)) != '\n')       /* read to end of line */
            if (c == EOF)
                return END;
        linecount++;
        return get_token();
    default:
        if (isalpha(c) || (c = '_'))    /* identifier returned in buffer */
        {
            int i = 0;

            /* buffer[i++] = tolower(c);		/* store only lowercase */
            buffer[i++] = c;
            while (is_ident_char(c = getc(input)))
            {
                if (i < BUFFER_SIZE)    /* bound check */
                    /* buffer[i++] = tolower(c);/* store only lowercase */
                    buffer[i++] = c;
                else
                {                       /* read rest of identifier */
                    while (is_ident_char(c = getc(input)));
                    ungetc(c, input);
                    parse_error("IDENTIFIER TO LONG");
                    return ERROR;
                }
            } ungetc(c, input);
            return IDENT;
        }
        else
        {
            parse_error("UNKNOWN TOKEN");
            return ERROR;
        }
    }
}

int skip_to_period()
{
    int token;

    while (token = get_token())
    {
        if ((token == PERIOD) ||
            (token == END))
            return token;
    }
    return token;
}

int skip_to_semicolon()
{
    int token;

    while (token = get_token())
    {
        if ((token == SEMICOLON) || (token == PERIOD) || (token == END))
            return token;
    }
    return token;
}

/* print linecount, filename and message */
void parse_error(char *message)
{
    printf(" ERROR AT LINE %d IN %s - %s\n", linecount, filename, message);
}

int parse_vocab(char *vocab_file)
{
    int more_att = 1, status = TRUE;
    identifier arg1 = {0}, arg2 = {0}, arg3 = {0}, arg4 = {0};

    filename = vocab_file;
    linecount = 1;
    input = fopen(filename,"r");

    if (input)
        printf("\n Parsing %s\n\n", vocab_file);
    else
    {
        printf("\n CAN'T OPEN %s\n\n", vocab_file);
        return FALSE;
    }

    /**************************/
    /* OBJECT TYPE DEFINITION */
    /**************************/

    while (1)
    {
        /* OBJECT TYPE NAME */

        switch (get_token())
        {
        case IDENT:
            strcpy(arg1, buffer);
            status &= add_obj_type(arg1);
            break;
        case DASH:
            goto do_rel_def;
        default:
            parse_error("EXPECTED OBJECT TYPE NAME OR DASH");
            fclose(input);
            return FALSE;
        }

        /* ATTRIBUTES NAME */

        do
        {
            switch (get_token())
            {
            case IDENT:

                strcpy(arg2, buffer);
                status &= add_att_name(arg1, arg2);

                switch (get_token())
                {
                case COMMA:
                    more_att = 1;
                    break;
                case SEMICOLON:
                    more_att = 0;
                    break;
                default:
                    parse_error("EXPECTED COMMA OR SEMICOLON");
                    fclose(input);
                    return FALSE;
                }
                break;
            case SEMICOLON:
                more_att = 0;
                break;
            default:
                parse_error("EXPECTED ATTRIBUTE NAME OR SEMICOLON");
                fclose(input);
                return FALSE;
            }
        }
        while (more_att);
    }

do_rel_def:

    /***********************/
    /* RELATION DEFINITION */
    /***********************/

    while (1)
    {
        /* DOMAIN */

        switch (get_token())
        {
        case IDENT:
            strcpy(arg1, buffer);
            break;
        case END:
            fclose(input);
            return status;
        default:
            parse_error("EXPECTED OBJECT TYPE");
            fclose(input);
            return FALSE;
        }

        /* RELATION */

        switch (get_token())
        {
        case IDENT:
            strcpy(arg2, buffer);
            break;
        default:
            parse_error("EXPECTED RELATION NAME");
            fclose(input);
            return FALSE;
        }

        switch (get_token())
        {
        case SLASH:
            switch (get_token())
            {
            case IDENT:
                strcpy(arg3, buffer);
                break;
            default:
                parse_error("EXPECTED INVERSE NAME");
                fclose(input);
                return FALSE;
            }

            /* INVERSE */

            switch (get_token())
            {
            case IDENT:
                strcpy(arg4, buffer);
                break;
            default:
                parse_error("EXPECTED OBJECT TYPE");
                fclose(input);
                return FALSE;
            }

            /* RANGE */

            switch (get_token())
            {
            case SEMICOLON:
                status &= add_rel_def(arg1, arg2, arg4, arg3);
                goto next_rel_def;
            default:
                parse_error("EXPECTED SEMICOLON");
                fclose(input);
                return FALSE;
            }

            break;

        case SEMICOLON:
            status &= add_rel_def(arg1, arg2, arg1, arg2);
            goto next_rel_def;

        default:
            parse_error("EXPECTED SLASH OR SEMICOLON");
            fclose(input);
            return FALSE;
        }
next_rel_def:;
    }
}

int parse_desc(char *desc_file)
{
    int more_rel_or_att, more_items;                    /* flags used to control loops */
    int status = TRUE;                  /* used to return status:
                                         * TRUE - no errors,
                                         * FALSE - errors! */

    /* used to store identifiers and pass params to data_manager functions */
    identifier arg1 = {0}, arg2 = {0}, arg3 = {0}, arg4 = {0};

    /* init globals */
    filename = desc_file;
    linecount = 1;
    input = fopen(filename,"r");

    if (input)
        printf("\n Parsing %s\n\n", desc_file);
    else
    {
        printf("\n CAN'T OPEN %s\n\n", desc_file);
        return FALSE;
    }

    while (1)
    {
        /* OBJECT TYPE */

        switch (get_token())
        {
        case IDENT:
            strcpy(arg1, buffer);
            break;
        case END:
            fclose(input);
            return status;
        default:
            parse_error("EXPECTED OBJECT TYPE");
            skip_to_period();
            status &= FALSE;
            goto next_object;
        }

        /* OBJECT NAME */

        switch (get_token())
        {
        case IDENT:
            strcpy(arg2, buffer);
            break;
        case END:
            parse_error("UNEXPECTED END OF FILE");
            fclose(input);
            return FALSE;
        default:
            parse_error("EXPECTED OBJECT NAME");
            skip_to_period();
            status &= FALSE;
            goto next_object;
        }

        status &= add_object(arg2, arg1);

        do
        {
            /* RELATIONSHIP OR ATTRIBUTE */

            switch (get_token())
            {
            case PERIOD:
                goto next_object;
            case IDENT:
                strcpy(arg3, buffer);
                break;
            case END:
                parse_error("UNEXPECTED END OF FILE");
                fclose(input);
                return FALSE;
            default:
                parse_error("EXPECTED RELATION OR ATTRIBUTE NAME");
                status &= FALSE;

                switch (skip_to_semicolon())
                {
                case PERIOD:
                    goto next_object;
                case SEMICOLON:
                    goto next_rel_or_att;
                case END:
                    fclose(input);
                    return FALSE;
                }
            }

            do
            {
                /* OBJECT OR TEXT */

                switch (get_token())
                {
                case IDENT:         /* object */

                    strcpy(arg4, buffer);

                    status &= add_relation(arg2, arg3, arg4);

                    switch (get_token())
                    {
                    case COMMA:
                        more_items = 1;
                        more_rel_or_att = 1;
                        break;
                    case SEMICOLON:
                        more_items = 0;
                        more_rel_or_att = 1;
                        break;
                    case PERIOD:
                        more_items = 0;
                        more_rel_or_att = 0;
                        break;
                    case END:
                        parse_error("UNEXPECTED END OF FILE");
                        fclose(input);
                        return FALSE;

                    default:
                        parse_error("EXPECTED COMMA, SEMICOLON OR PERIOD");
                        status &= FALSE;

                        switch (skip_to_semicolon())
                        {
                        case PERIOD:
                            goto next_object;
                        case SEMICOLON:
                            goto next_rel_or_att;
                        case END:
                            fclose(input);
                            return FALSE;
                        }
                    }
                    break;

                case STRING:            /* text */

                    strcpy(arg4, buffer);

                    status &= add_attribute(arg2, arg3, arg4);

                    switch (get_token())
                    {
                    case SEMICOLON:
                        more_items = 0;
                        more_rel_or_att = 1;
                        break;
                    case PERIOD:
                        more_items = 0;
                        more_rel_or_att = 0;
                        break;
                    case END:
                        parse_error("UNEXPECTED END OF FILE");
                        fclose(input);
                        return FALSE;
                    default:
                        parse_error("EXPECTED SEMICOLON OR PERIOD");
                        status &= FALSE;

                        switch (skip_to_semicolon())
                        {
                        case PERIOD:
                            goto next_object;
                        case SEMICOLON:
                            goto next_rel_or_att;
                        case END:
                            fclose(input);
                            return FALSE;
                        }
                    }
                    break;

                case END:
                    parse_error("UNEXPECTED END OF FILE");
                    fclose(input);
                    return FALSE;

                default:
                    parse_error("EXPECTED OBJECT OR TEXT");
                    status &= FALSE;

                    switch (skip_to_semicolon())
                    {
                    case PERIOD:
                        goto next_object;
                    case SEMICOLON:
                        goto next_rel_or_att;
                    case END:
                        fclose(input);
                        return FALSE;
                    }
                }
            }
            while (more_items);
next_rel_or_att:;
        }
        while (more_rel_or_att);
next_object:;
    }
}
