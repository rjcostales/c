/* Raphael J.S. Costales
 * Token.h 3/20/93
 */

#include <iostream>
#include "Token.h"

char Token::matrix[3][3] = {	// Initialize static matrix
							{ ' ', ' ', ' ' },
							{ ' ', ' ', ' ' },
							{ ' ', ' ', ' ' }
						};

int Token::move(int i)			// use digit input so it works with
{								// keypad entry and ModTree storage.
	switch (i)					// ugly but effective
	{
		case 1: if (matrix[0][0] == ' ') matrix[0][0] = mC;
				else i = 0;
				break;
		case 2: if (matrix[0][1] == ' ') matrix[0][1] = mC;
				else i = 0;
				break;
		case 3: if (matrix[0][2] == ' ') matrix[0][2] = mC;
				else i = 0;
				break;
		case 4: if (matrix[1][0] == ' ') matrix[1][0] = mC;
				else i = 0;
				break;
		case 5: if (matrix[1][1] == ' ') matrix[1][1] = mC;
				else i = 0;
				break;
		case 6: if (matrix[1][2] == ' ') matrix[1][2] = mC;
				else i = 0;
				break;
		case 7: if (matrix[2][0] == ' ') matrix[2][0] = mC;
				else i = 0;
				break;
		case 8: if (matrix[2][1] == ' ') matrix[2][1] = mC;
				else i = 0;
				break;
		case 9: if (matrix[2][2] == ' ') matrix[2][2] = mC;
				else i = 0;
				break;
		default: i = 0;
	}
	return i;
}

int Token::count()
{
	int	  c = 0;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (matrix[i][j] == mC)
				c++;
	return c;
}

int Token::win()
{	// check rows and columns
	for (int i = 0; i < 3; i++)
		if (((matrix[i][0] == mC)&&(matrix[i][1] == mC)&&(matrix[i][2] == mC))||
			((matrix[0][i] == mC)&&(matrix[1][i] == mC)&&(matrix[2][i] == mC)))
			return 1;
	// check diagonals
	if (((matrix[0][0] == mC)&&(matrix[1][1] == mC)&&(matrix[2][2] == mC))||
		((matrix[0][2] == mC)&&(matrix[1][1] == mC)&&(matrix[2][0] == mC)))
		return 1;
	return 0;
}

void Token::fill()
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			matrix[i][j] = mC;
}

