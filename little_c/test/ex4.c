/* This program demonstrates recursive functions. */

main()
{
	print(factr(10));
}

/* return the factorial of i */
factr(int i)
{
	if (i < 2) {
		return 1;
	} else {
		return i * factr(i - 1);
	}
}
