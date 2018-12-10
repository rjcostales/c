/* binary search example  */
#include <stdio.h>
#include <stdbool.h>

void main()
{
    int high, lo, mid, found = false;
    int target = 367;
    int a[1000];

    /* seed the array */
    for(int i =0; i<1000; i++)
        a[i]=i;

    lo = 0;
    high = 1000;
    mid = (high + lo) / 2;

    while (lo < high) {
        printf("%d\n", mid);
        if (target <= a[mid])
            high = mid;
        else
            lo = mid + 1;

        mid = (high + lo) / 2;
    }

    if (a[mid] == target)
        found = true;

    if (found)
        printf("found target: %d\n", target);
    else
        printf("target %d not found\n", target);

}
