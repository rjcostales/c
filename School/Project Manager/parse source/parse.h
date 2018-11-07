#ifndef __DROPUNIX_MAIN__
#define __DROPUNIX_MAIN__

int Main(int argc, char ** argv);
#define main Main

#endif /* __DROPUNIX_MAIN__ */

enum
{
    IN, OUT
};

typedef enum
{
    none, inner, outer, subroutine, global
} type;

typedef struct node
{
    char            name[64];
    struct node    *left, *right;
} node, *tree;

int iskeyword(char[]);
void insert(tree *, char[]);
void print(tree);
tree locate(tree, char[]);
int  main(int, char **);

