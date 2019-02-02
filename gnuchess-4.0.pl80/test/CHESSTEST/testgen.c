#include <stdio.h>
#include <strings.h>
/* convert fen notation to gnu save format */
char            b[8][8];
char            fn[12],*color, *colort;
int             i, j, k, l;
int             r, c;
int		eps;
int timec,timem,times;
char           *p, *q;
FILE           *F, *O, *W, *R;
int             wq, wk, bq, bk;
char            in[512];
char            move[512],*q;
/*						*/
/*		testgen file testx bk time-min moves time-sec */
/* 						*/
int count = 0;
int countx = 0;
char name[100];
char namer[100];
main(argc,argv)
int argc;
char *argv[];
{
	k = 1;
	F = fopen(argv[1], "r");
/*
	W = fopen(argv[3], "w");

	fprintf(W,"0 0\n");
*/
	timec = atoi(argv[4])*6000;
	timem = atoi(argv[5]);
	times = atoi(argv[6]);
        timec=timec+ ( times*100);

count =100;
	while (fgets(in, 512, F) != NULL) {
		if(strncmp("begin",in,4) == 0)continue;
		if(strncmp(in,"\n",1) == 0)continue;
		if(strncmp(in,"#",1) == 0)continue;
		if(strncmp(in,"\t",1) == 0)continue;
		if (strncmp(" ", in, 1) != 0) {
		count++;
	if(count > 99){
		count = 0; countx++;
	        sprintf(name,"%s%02d",argv[3],countx);
	        sprintf(namer,"%s.%s%02d.ANS",argv[1],argv[3],countx);
if(countx > 1){
		fprintf(W,"quit\n"); fclose(W);}
		 W = fopen(name,"w");
		 R = fopen(namer,"w");
		fprintf(W,"0 0\n");
	}
	fprintf(W,"get\n");
			wq = wk = bq = bk = 10;
			for (i = 0; i < 8; i++) {
				for (j = 0; j < 8; j++) {
					b[i][j] = '.';
				}
			}
			p = in;
			r = 0;
			c = 0;
			while (r<8 && *p != ' ') {
				if (isdigit(*p)) {
					c += (*p - '0');
				} else if (*p == '/') {
					r++;
					c = 0;
				} else {
					if(isupper(*p))*p = tolower(*p);
					else *p = toupper(*p);
					b[r][c] = *p;
					c++;
				}
				p++;

			}
			if(*p == 'w') {color = "White";colort = "white";}
			 else {color = "Black";colort = "black";}
			p += 2;
			sprintf(fn, "%s%03d", argv[2],k);
/* at Right move */
			q = move;
			while(*p != ' '){ 
				if(isupper(*p)){p++; continue;}
				if(*p == 'x'){ p++; continue;}
				if(*p == '+'){ p++; continue;}
				if(*p == '!'){ p++; continue;}
				if(*p == '?'){ p++; continue;}
				if(*p == '='){ p++; continue;}
				*q++ = *p++;
			}
			*q++ = '\0';
			fprintf(R,"My move is: %s\n",move);
			k++;
			O = fopen(fn, "w");
			fprintf(W, "%s\n",fn);
			fprintf(W, "level %d %d:%d\n",timem,timec/6000,timec%6000);
			fprintf(W,"%s\npost\ngo\n",colort);
			fprintf(O, "White computer Black Human 1 eps %d # To move %s correct %s\nCastled White false Black false # ep \nTimeControl 0 Operator Time 0\nWhite Clock %d Moves %d\nBlack Clock %d Moves %d\n\n", eps,color,move,timec,timem,timec,timem);
			for (r = 0; r < 8; r++) {
				fprintf(O, "%c ", '0' + r);
				for (c = 0; c < 8; c++) {
					fprintf(O, "%c", b[r][c]);
				}
				if (r == 0)
					fprintf(O, " %d 10 10 10 %d 10 10 %d\n", bq, ((bq + bk) > 10) ? 10 : 0, bk);
				else if (r == 7)
					fprintf(O, " %d 10 10 10 %d 10 10 %d\n", wq, ((wq + wk) > 10) ? 10 : 0, wk);
				else
					fprintf(O, " 10 10 10 10 10 10 10 10\n");
			}
			fprintf(O, "  abcdefgh\n\nmove  score depth  nodes  time flags capture color\n");
			fclose(O);
	}
}
fprintf(W, "quit\n");
fclose(W);
fclose(R);
}
