#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
	char buffer[1024];
	char *token, *quote, *comma;

	while (fgets(buffer, 1024, stdin)) {
		token = buffer;

		while (*token) {

			while (isspace(*token))
				token++;
			if (*token == '"') {
				quote = strchr(token + 1, '"');
				comma = strchr(quote, ',');
			 } else
				comma = strchr(token, ',');
	 
			 if (comma) {
				 *comma = '\0';
				 printf("%s\t", token);
				 token = comma + 1;
			} else {
				  printf("%s", token);
				  break;
			 }
		}
	}

	return 0;
}
