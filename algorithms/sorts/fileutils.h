#define MAXLINE 1024
#define MAXSIZE 65536

#define ELAPSE_TIME(X) ((float) (X) / (float) CLOCKS_PER_SEC)

typedef char *str;

int read(str strings[]); // return the number of lines

void write(str strings[]);
