/* Raphael J.S. Costales
 * Token.h 3/20/93
 *
 * Class to alter and display TicTacToe matrix
 * Also determines loses, draw
 */
#include <iostream>

class Token
{
public:
	int move(int);				// places mC using digit input in matrix
	int count();				// return the number mC in matrix
	int win();					// returns true if token has mC in a row
	void fill();				// fills matrix with mC
	static void print()			// static function to draw matrix
	{							// matrix drawn upside down to match keypad
		for (int i = 2; i >= 0; i--)
		{
			cout << ' ' << matrix[i][0] << " |";
			cout << ' ' << matrix[i][1] << " |";
			cout << ' ' << matrix[i][2] << '\n';
			if (i)
				cout << "---+---+---\n";
		}
	}
	// constructor
	Token(char C) { mC = C; }
private:
	char mC;
	static char matrix[3][3];
};
