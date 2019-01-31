/*	Raphael J. S. Costale
 *	DimWit.h
 *	5/5/93
 */
#ifndef DIMWIT_H
#define DIMWIT_H
#include <ostream.h>
#include"Runner.h"
#include"Swamp.h"

class DimWit : public Runner
{
public:
// constructor
	DimWit(Swamp&);
// member functions
	void draw(int);
	void setPos(Coord);

protected:
	Coord step();  // value returned by virtual function move

	Swamp *mSwamp;
};

//	implementation
DimWit::DimWit(Swamp& swamp)
{
	mSwamp = &swamp;
	mMoves = mAttemps = 0;
}

void DimWit::draw(int isSafe)
{
	cout << (isSafe ? 'D' : '*');
}

void DimWit::setPos(Coord p)
{
	Runner::setPos(p);
}

Coord DimWit::step()
{
	Coord temp = mPos;
	do
	{
		mPos = temp;
		Runner::step();
	} while (!mSwamp->inSwamp(mPos));	// check if in swamp

	return mPos;
}

#endif

// EOF
