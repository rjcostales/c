#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char buffer[1024];

    memset(buffer, 0, sizeof(buffer));
    
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        printf("%lu\n", strlen(buffer) - 1);
    }

    return 0;
}
