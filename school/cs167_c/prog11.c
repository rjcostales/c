/*
 * Jesse Costales
 * Asgn 5 - Program 11, opt A
 *
 * This program was compiled in to a file named code
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX     20              /* max number of words in list */
#define SIZE    50              /* max size of word */

/* prototypes */
void    sort(char **, int);
void    print(char **, int, char *);
void    code_list(char **, int, char);
void    encode(char *);
void    decode(char *);
void    print_error(void);

int main(int argc, char* argv[])
{
    FILE *input;
    char *mode, *filename;
    int i = 0, j;
    char *list[MAX];
    char word[SIZE];
    if (argc != 3)                  /* check for correct number of parameters */
    {
        print_error();
        return 0;
    }
    mode = *(argv + 1);             /* check for correct options */
    if (mode[0] != '-')
    {
        print_error();
        return 0;
    }
    switch (mode[1])
    {
    case 'd':
        printf("Decoding file ...\n");
        break;
    case 'e':
        printf("Encoding file ...\n");
        break;
    default:
    {
        print_error();
        return 0;
    }
    }
    filename = *(argv + 2);         /* get filename and check if it exist */
    input    = fopen(filename, "r");
    if (input == NULL)
    {
        printf("%s:  No such file or directory\n", filename);
        return 0;
    }
    while (fscanf(input, "%s", word) == 1)
    {
        list[i] = calloc(strlen(word) + 1, sizeof(char));
        strcpy(list[i++], word);
        if (i >= MAX)                   /* don't read too many words */
        {
            printf("Too many words, list will be truncated\n");
            break;
        }
    }
    sort(list, i);
    print(list, i, "\nSorted list");
    code_list(list, i, mode[1]);
    print(list, i, (mode[1] == 'd' ? "\nDecoded list" : "\nEncode list"));
}

void sort(char **list, int size)        /* list is ragged array of strings of size */
{
    int i, j;
    char *temp;
    for (i = 0; i < size; ++i)
        for (j = i + 1; j < size; ++j)
            if (strcmp(*(list + i), *(list + j)) > 0)
            {
                temp        = *(list + i);
                *(list + i) = *(list + j);
                *(list + j) = temp;
            }
}

void print(char **list, int size, char *title)  /* print ragged array with a
                                                 * title */
{
    int i;
    puts(title);
    for (i = 0; i < size; i++)
        puts(list[i]);
}

void code_list(char **list, int size, char m)   /* encode the words in list
                                                 * if m == 'e' otherwise
                                                 * decode */
{
    int i;
    for (i = 0; i < size; i++)
        if (m == 'e')
            encode(list[i]);
        else
            decode(list[i]);
}

void encode(char *word)         /* called by code_list */
{
    int i;
    for (i = 0; word[i]; i++)
        if (isalpha(word[i]))
            if (islower(word[i]))
                word[i] = ('z' + 'a' - word[i]);
            else if (word[i] == 'Z')
                word[i] = 'A';
            else
                word[i] = (word[i] + 1);
}

void decode(char *word)         /* called by code_list */
{
    int i;
    for (i = 0; word[i]; i++)
        if (isalpha(word[i]))
            if (islower(word[i]))
                word[i] = ('z' + 'a' - word[i]);
            else if (word[i] == 'A')
                word[i] = 'Z';
            else
                word[i] = (word[i] - 1);
}

void print_error()                      /* print usage errror */
{
    printf("Usage:  code [-de] filename\n");
    printf("        -d      decode file\n");
    printf("        -e      encode file\n");
}
