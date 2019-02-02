#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char buffer[1024];
    int counts[256] = { 0 };

    memset(buffer, 0, sizeof(buffer));

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        int l = strlen(buffer);
        for (int i = 0; i < l; i++) {
            char c = buffer[i];
            if (c >= 'a')
                c -= 32;
            counts[c]++;
        }
    }

    for (int i = 0; i < 128; i++)
        if (counts[i] > 0)
            printf("%c\t%0.0f\n", i, counts[i] / 953.0);

    return 0;
}
