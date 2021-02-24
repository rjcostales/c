/* strtok example */
#include <stdio.h>
#include <string.h>

int main ()
{
	char str[] ="This is a comma, delimeted, file";
	char *pch;

	printf ("Splitting string \"%s\" into tokens:\n",str);
	pch = strtok (str,",");

	while (pch != NULL)
	{
		printf ("%s\n",trim(pch));
		pch = strtok (NULL, ",");
	}
	return 0;
}
