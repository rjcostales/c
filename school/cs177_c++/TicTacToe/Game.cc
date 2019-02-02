/* Raphael J.S. Costales
 * Game.cc #/20/93
 *
 * Main function of TicTacToe Game.	 Game uses two classes ModTree and Token
 * which communicate through member functions.	There are a couple "goto's"!
 * Oh well...
 */

#include <ctype>
#include <stdlib>
#include "ModTree.h"
#include "Token.h"

int main()
{
	Token X('X'); Token O('O'); Token Blank(' ');
	ModTree Computer;
	int move;

	Computer.load("data");
	
	cout << "Use keypad to enter moves\n";
	cout << " 7 | 8 | 9 \n";
	cout << "---+---+---\n";
	cout << " 4 | 5 | 6 \n";
	cout << "---+---+---\n";
	cout << " 1 | 2 | 3 \n";
	cout << "Enter 0 to quit\n\n";

	Computer.learn();

loop:

	Blank.fill();

	while (Blank.count())
	{
		Token::print();

		do
		{
			cout << "Enter move ";
			cin >> move;
			if (!move) goto pass;
		}
		while (X.move(move) == 0);

		Computer.putX(move);

		if (X.win())
		{
			Computer.forget();
			Token::print();
			cout << "You win\n\n";
			break;
		}

		if (!Blank.count())
		{
			Computer.learn();
			Token::print();
			cout << "Draw\n\n";
			break;
		}

		O.move(Computer.getO());

		if (O.win())
		{
			Computer.learn();
			Token::print();
			cout << "You lose!!!\n\n";
			break;
		}
	}

	goto loop;

pass:
	
	return 1;
}

