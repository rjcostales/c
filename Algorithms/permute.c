/* prototype for recursive permutation */

#include <stdio.h>

int array[4] = { 1, 2, 3, 4 };

void swap(int i, int j) {
    int k;

    k = array[i];
    array[i] = array[j];
    array[j] = k;
}

void permute(int i) {

    int j, k;

    if (j == i) {
        while (j--) {
            swap(i - 1, j);
            permute(i - 1);
            swap(j, i - 1);
        }
    } else {

        k = 4;

        while (k--) {
            printf("%d", array[k]);
        }

        printf("\n");
    }
}

main() {
    permute(4);
}
