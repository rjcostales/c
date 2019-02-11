/* A more rigorous example of function arguments. */

main()
{
    print("in main");
    print(f2(f1(3), f1(4)));
}

f1(int a)
{
    return a * a;
}

f2(int x, int y)
{
    return x + y;
}
