/* Module:	controller.c
 *
 * Purpose: This module controls the sequence of operations and contains the user interface
 * functions to gather the user choices.
 *
 * Written by Jesse Costales, Ismael Flores, Julian Escontrias, and Mike Marez.
 *
 * Note: This module is reponsible for initializing variables upon startup and clean up before
 * exiting back to the system.
 *
 */

#include "project_manager.h"

/*	GLOBALS */

list attribute_names = NULL;
list relation_names = NULL;
list undeclared = NULL;
list unused_globals = NULL;
list unused_subroutines = NULL;

ot_tree object_types = NULL;
rd_tree relation_defs = NULL;

object Object_Table[MAX_OBJECTS] = {NULL};
relationship Relation_Table[MAX_RELATIONS] = {NULL};

main()
{
    dispose_vocab();
    dispose_desc();
    MenuTitle();
    MenuMain();
    return 0;
}

/*	Private Functions of the Controller */

void wait(void)
{
    char response[10];

    printf("\n Press enter to continue ");
    gets(response);
}

void InputDesc(void)
{
    char filename[32];

    printf("\n Read in Description File.");
    printf("\n Enter a valid File Name: ");

    gets(filename);

    if (is_empty_vocab())
    {
        printf(" No vocabulary definition!\n");
        return;
    }

    if (!parse_desc(filename))
    {
        printf("\n ERRORS DETECTED IN %s\n", filename);
        dispose_desc();
        wait();
        return;
    }
}

void InputVocab(void)
{
//	  char			  filename[32];

//	  printf("\n Read in Vocabulary File.");
//	  printf("\n Enter a valid File Name: ");

//	  gets(filename);
//	  if (!strlen(filename))
//		 strcpy(filename,"vocabulary");

    dispose_vocab();
    dispose_desc();

    if (!parse_vocab("vocabulary"))
    {
        printf("\n ERRORS DETECTED IN %s\n", "vocabulary");
        dispose_vocab();
        wait();
        return;
    }
}

void MenuTitle(void)
{
    printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
           "**************************************************",
           "**                                              **",
           "**                PROJECT MANAGER               **",
           "**                                              **",
           "**                  Version 1.5                 **",
           "**                                              **",
           "**************************************************");

    while (is_empty_vocab())
        InputVocab();

//	  wait();

    while (is_empty_desc())
        InputDesc();

    update_relations();
    wait();
}

void MenuMain(void)
{
    char choice[2];

    system("clear");
    printf("\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
           "   MAIN MENU",
           "===================================================",
           "   (1). Read in a New Vocabulary File              ",
           "   (2). Read in a New Description File             ",
           "   (3). Metrics Menu                               ",
           "   (4). Print Menu                                 ",
           "   (5). EXIT                                       ",
           "===================================================");
    printf("\n Make a selection: ");
    gets(choice);

    switch (atoi(choice))
    {
    case 1:
        /*	Read in a new vocabulary file */
        dispose_vocab();
        while (is_empty_vocab())
            InputVocab();
        wait();

    case 2:                         /*	Read in a new description file */
        dispose_desc();
        while (is_empty_desc())
            InputDesc();
        update_relations();
        wait();
        break;

    case 3:                         /*	Display Metrics Menu */
        MenuMetrics();
        break;

    case 4:                         /*	Display Print Menu */
        MenuPrint();
        break;

    case 5:                         /*	To exit out of the program manager */
        printf(" Type y to exit ");
        if (tolower(getchar()) == 'y')
            exit(1);
        break;

    default:                        /*	Any other selection will display main menu */
        MenuMain();
    }

    MenuMain();
}

void MenuMetrics(void)
{
    char choice[2];
    char type[32];
    char relation[32];
    char path[32];

    system("clear");
    printf("\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
           "   METRICS MENU",
           "===================================================",
           "   (1). Print McCabe Complexity                    ",
           "   (2). Print Henry Kafura Number                  ",
           "   (3). Current State of the Project               ",
           "   (4). Return to MAIN MENU                        ",
           "===================================================");

    printf("\n Make a selection: ");
    gets(choice);

    switch (atoi(choice))
    {
    case 1:                         /*	Displays the McCabe Complexity */
        printf(" What object type do you want, default is function? ");
        gets(type);

        printf(" What attribute do you want, default is path? ");
        gets(path);

        if (!strlen(type))
            strcpy(type,"function");

        if (!strlen(path))
            strcpy(path,"path");

        compute_complexity(type, path);
        wait();
        break;

    case 2:                         /*	Displays Henry Kafura Number */
        printf(" What object type do you want, default is function? ");
        gets(type);

        printf(" What relation do you want, default is calls? ");
        gets(relation);

        if (!strlen(type))
            strcpy(type,"function");

        if (!strlen(relation))
            strcpy(relation,"calls");

        compute_calls(type, relation);
        wait();
        break;

    case 3:                         /*	Displays information about current state of the project */
        printf(" What object type do you want, default is function? ");
        gets(type);

        printf(" What relation do you want, default is calls? ");
        gets(relation);

        if (!strlen(type))
            strcpy(type,"function");

        if (!strlen(relation))
            strcpy(relation,"calls");

        compute_levels(type, relation);
        wait();
        break;

    case 4:                         /*	Displays Main Menu */
        MenuMain();
        break;

    default:                        /*	Any other selection will display metrics menu */
        MenuMetrics();
    }

    MenuMetrics();
}

void MenuPrint(void)
{
    char choice[2];
    char filename[32];

    system("clear");
    printf("\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
           "   PRINT MENU",
           "===================================================",
           "   (1). Print Objects Alphabetically               ",
           "   (2). Print Objects by Type                      ",
           "   (3). Print Objects by Relation                  ",
           "   (4). Print Vocabulary                           ",
           "   (5). Print Description File                     ",
           "   (6). Print Levels                               ",
           "   (7). Return to MAIN MENU                        ",
           "===================================================");

    printf("\n Make a selection: ");
    gets(choice);

    if ((atoi(choice) < 7) && (atoi(choice) > 0))
    /*	Provides user to save print output in a file */
    {
        printf(" Enter filename or enter to print to screen: ");
        gets(filename);
    }

    switch (atoi(choice))
    {
    case 1:                         /*	Displays Objects Alphabetically */
        report_obj_by_name(filename);
        wait();
        break;

    case 2:                         /*	Displays Objects by Type */
        report_obj_by_type(filename);
        wait();
        break;

    case 3:                         /*	Displays Objects by Relation */
        report_relations(filename);
        wait();
        break;

    case 4:                         /*	Displays Description file */
        report_vocab(filename);
        wait();
        break;

    case 5:                         /*	Displays Description file */
        report_desc(filename);
        wait();
        break;

    case 6:                         /*	Displays level file */
        report_levels(filename);
        wait();
        break;

    case 7:
        MenuMain();

    default:                        /*	Any other selection will display print menu */
        MenuPrint();
    }

    MenuPrint();
}
