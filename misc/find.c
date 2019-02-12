#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int substr(char * str, char * sub)
{
    while (*sub) {
        if (*sub++ != *str++)
            return 0;
    }
    return 1;
}

int main(int argc, char *argv[])
{
    char line[256];
    int i;
    char test[] = "test";

    while (fgets(line, 256, stdin)) {
        for (i = 0; line[i]; i++) {
            if (i == 0) {
                if ((substr(&line[i], test)) &&
                  (!isalpha(line[i + strlen(test)])))
                    puts(line);
            } else {
                if ((!isalpha(line[i - 1])) && (substr(&line[i], test)) &&
                  (!isalpha(line[i + strlen(test)])))
                    puts(line);
            }
        }
    }
}
