#include <stdlib.h>
#include <stdio.h>
// #include <conio.h>
 unsigned int Find (int *array, unsigned int size, int element);
void Permute (const int *p, const unsigned int size);
 unsigned int n = 0;
 int
main ()  {
  int array[7] = { 1, 2, 3, 4, 5, 6, 7 };
   Permute (array, 7);
  printf ("Total of %d permutations(ways to write) found\n", n);
  system ("pause");
  return 1;
}

 void
Permute (const int *p, const unsigned int size)  {
  static int *cp = new int[size];
  static int cpindex = 0;
   if (cpindex >= size)
     {
    for (int z = 0; z < size; z++)
      printf ("%d", cp[z]);
     printf ("\n");
     cpindex--;
    n++;
     
      //getch();
      return;
     }
   for (int z = 0; z < size; z++)
      {
     if (Find (cp, cpindex, p[z]) != -1)
       continue;
     cp[cpindex++] = p[z];
     
      //if ()
      Permute (p, size);
     }
   cpindex--;
 }

 unsigned int
Find (int *array, unsigned int size, int element)  {
   for (int i = 0; i < size; i++)
     {
     if (array == element)
       return i;
     }
   return -1;
 }


