/* The loop statements. */

main()
{
    int a;
    char ch;

    /* the while */
    puts("Enter a number greater than 1:");
    a = getnum();

    while (a) {
        print(a * a);
        a = a - 1;
    }

    /* the do-while */
    puts("enter characters, 'q' to quit");
    do {
        ch = getche();
        print(ch);
    } while (ch != 'q');

    /* the for */
    puts("for loop:");
    for (a = 0; a < 3; a = a + 1) {
        print(a);
    }
}
