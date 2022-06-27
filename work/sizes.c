#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>
#include <locale.h>

int main(int argc, char *argv[])
{
	setlocale(LC_NUMERIC, "");
	printf("size of int      :%u\n", (int)sizeof(int));
	printf("size of char     :%u\n", (int)sizeof(char));
	printf("size of short    :%u\n", (int)sizeof(short));
	printf("size of long     :%u\n", (int)sizeof(long));

	printf("size of float    :%u\n", (int)sizeof(float));
	printf("size of double   :%u\n", (int)sizeof(double));

	printf("size of uint32_t :%u\n", (int)sizeof(uint32_t));
	printf("size of uint64_t :%u\n", (int)sizeof(uint64_t));

	printf("\nshort\n");
	printf("short max     :%'i\n", SHRT_MAX);
	printf("short min     :%'i\n", SHRT_MIN);
	printf("short unsiged :%'u\n", USHRT_MAX);

	printf("short decimal :%'d\n", USHRT_MAX);
	printf("short hex     :0x%x\n", USHRT_MAX);
	printf("short octal   :o%o\n", USHRT_MAX);

	printf("\nint\n");
	printf("int max       :%'i\n", INT_MAX);
	printf("int min       :%'i\n", INT_MIN);
	printf("int unsiged   :%'u\n", UINT_MAX);

	printf("int decimal   :%d\n", UINT_MAX);
	printf("int hex       :0x%x\n", UINT_MAX);
	printf("int octal     :o%o\n", UINT_MAX);

	printf("\nlong\n");
	printf("long max      :%'li\n", LONG_MAX);
	printf("long min      :%'li\n", LONG_MIN);
	printf("long unsiged  :%'lu\n", ULONG_MAX);

	printf("long decimal  :%'ld\n", ULONG_MAX);
	printf("long hex      :0x%lx\n", ULONG_MAX);
	printf("long octal    :o%lo\n", ULONG_MAX);

	printf("\nfloating point\n");
	printf("float  :%le\t%le\n", FLT_MIN, FLT_MAX);
	printf("double :%le\t%le\n", DBL_MIN, DBL_MAX);

	printf("float  :%f\n", FLT_MIN);
	printf("double :%f\n", DBL_MIN);

	printf("float  :%f\n", FLT_MAX);
	printf("double :%f\n", DBL_MAX);
}
