/*	Raphael J. S. Costale
 *	Genius.h
 *	5/5/93
 */
#ifndef GENIUS_H
#define GENIUS_H
#include <ostream.h>
#include"Wit.h"

class Genius : public Wit
{
public:
// constructor
	Genius(Swamp&);
// member functions
	void draw(int);
	void setPos(Coord);
protected:
	Coord step();  // value returned by virtual function move
};

//	implementation
Genius::Genius(Swamp& swamp) : Wit(swamp) {}
void Genius::draw(int isSafe)
{
	cout << (isSafe ? 'G' : '*');
}

void Genius::setPos(Coord p)
{
	Runner::setPos(p);
}

Coord Genius::step()
{
	Coord temp = mPos;
	do
	{
		mPos = temp;
		Wit::step();
	} while (!mSwamp->isSafe(mPos));	// check if step is safe
	return mPos;
}

#endif
// EOF
