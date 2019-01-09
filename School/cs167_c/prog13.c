/*
 * Jesse Costales
 * Final Asgn - Prog 13
 *
 * Usage: command [input] [output]
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int value;
    struct  node *next;
}   node;

void    insert_node(node **, int);
int     list_length(node *);
int     access_node(node *, int);
void    print_list(node *, char *);
void    print_reverse(node *, char *);
void    reverse_print(node *);
void    delete_node(node **, int);
FILE *input, *output;

int main(int argc, char* argv[])
{
    int i;
    int num_values;
    int value;
    node *head = NULL;
    input  = stdin;
    output = stdout;
    switch (argc)           /* input and output file given */
    {
    case 3:
    {
        output = fopen(*(argv + 2), "r");
        if (output != NULL)
        {
            printf("%s exits, overwrite it [y/n]?", *(argv + 2));
            if (tolower(getchar()) != 'y')
                return 0;
        }
        fclose(output);
        output = fopen(*(argv + 2), "w");
    }                       /* break omitted to open input file */
    case 2:                 /* only input file given */
    {
        input = fopen(*(argv + 1), "r");
        if (input == NULL)
        {
            printf("%s doesn't exits!\n", *(argv + 1));
            return 0;
        }
    }
    case 1:                     /* no files given, use defaults */
        break;
    default:                    /* too many arguments */
    {
        printf("Usage: %s [input] [output]\n", *argv);
        return 0;
    }
    }
    fscanf(input, "%d", &num_values);       /* read values for list */
    for (i = 0; i < num_values; i++)
    {
        fscanf(input, "%i", &value);
        insert_node(&head, value);
    }
    fprintf(output, "Number of elements in list is %i\n", list_length(head));
    print_list(head, "Sorted list");
    fprintf(output, "\n");
    fscanf(input, "%d", &num_values);       /* read values to access */
    fprintf(output, "Access number and value\n");
    for (i = 0; i < num_values; i++)
    {
        fscanf(input, "%i", &value);
        fprintf(output, "%2d:%4d\n", value, access_node(head, value));
    }
    fprintf(output, "\n");
    fscanf(input, "%d", &num_values);               /* read value to delete */
    for (i = 0; i < num_values; i++)
    {
        fscanf(input, "%i", &value);
        delete_node(&head, value);
    }
    fprintf(output, "No. elem. in deleted list is %i\n", list_length(head));
    print_list(head, "sorted");
    fprintf(output, "\n");
    print_reverse(head, "reverse");
}

void insert_node(node ** head, int val)
{
    node *p, *lag = NULL, *q;
    if (*head == NULL)
    {
        *head          = (node *) malloc((size_t) sizeof(node));
        (*head)->value = val;
        (*head)->next  = NULL;
        return;
    }
    p = *head;
    while ((p != NULL) && (p->value < val))
    {
        lag = p;
        p   = p->next;
    }
    if (p != NULL)          /* don't access structure if NULL */
        if (p->value == val)
            return;
    if (lag == NULL)
    {
        *head          = (node *) malloc((size_t) sizeof(node));
        (*head)->value = val;
        (*head)->next  = p;
    }
    else
    {
        q         = (node *) malloc((size_t) sizeof(node));
        q->value  = val;
        q->next   = p;
        lag->next = q;
    }
}

int list_length(node * list)
{
    int i;
    for (i = 0; list != NULL; i++)
        list = list->next;
    return i;
}

void print_list(node * list, char *title)
{
    fprintf(output, "%s\n", title);
    while (list != NULL)
    {
        fprintf(output, "%d\n", list->value);
        list = list->next;
    }
}

int access_node(node * list, int n)
{
    if (n > list_length(list)) /* check if in range */
        return -1;
    while (--n)                /* work backwards */
        list = list->next;
    return list->value;
}

void print_reverse(node * list, char *title)
{                                   /* intermediate call to print title */
    fprintf(output, "%s\n", title); /* call recursive function */
    reverse_print(list);
}

void reverse_print(node * list)
{
    if (list->next != NULL)
        reverse_print(list->next);
    fprintf(output, "%d\n", list->value);
}

void delete_node(node ** head, int num)
{
    node *p, *lag = NULL;
    if (num > list_length(*head))
        return;                /* check to see if in range */
    p = *head;
    if (num == 1)              /* special case */
    {
        *head = (**head).next; /* different notation for practice */
        free(p);               /* free memory */
        return;
    }
    while (--num)           /* find node */
    {
        lag = p;
        p   = (*p).next;
    }
    (*lag).next = (*p).next;        /* repoint pointers */
    free(p);
}
