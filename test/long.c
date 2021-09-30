#include <stdio.h>
#include <locale.h>

int main()
{
   int       i = -1;
   long      l = -1;
   long long ll = -1;

   setlocale(LC_NUMERIC, "");
   printf("%i %i %i\n", (int) sizeof(i), (int) sizeof(l),
          (int) sizeof(ll));
}
