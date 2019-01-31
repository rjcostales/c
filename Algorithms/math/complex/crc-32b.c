/*------------------------------ crc.h ----------------------*/
#ifndef FILE                    /* if FILE type not defined */
#include <stdio.h>              /* then bring it in         */
#endif

/* generate the crc table. Must be called before calculating the crc value */
void    gen_table(void);
unsigned long get_crc(FILE *);  /* calculate the crc32 value */

/*-----------------------------------------------------------*/

/*-------------------------- crc.c --------------------------*/

static unsigned long crc_table[256];

void gen_table(void)
{                               /* build the crc table */
    unsigned long crc, poly;
    int i, j;

    poly = 0xEDB88320L;
    for (i = 0; i < 256; i++) {
        crc = i;
        for (j = 8; j > 0; j--) {
            if (crc & 1)
                crc = (crc >> 1) ^ poly;
            else
                crc >>= 1;
        }
        crc_table[i] = crc;
    }
}

unsigned long get_crc(FILE * fp)
{                               /* calculate the crc value */
    register unsigned long crc;
    int char;

    crc = 0xFFFFFFFF;
    while ((char = getc(fp)) !=EOF)
        crc = ((crc >> 8) & 0x00FFFFFF) ^ crc_table[(crc ^ char) &0xFF];

    return (crc ^ 0xFFFFFFFF);
}
