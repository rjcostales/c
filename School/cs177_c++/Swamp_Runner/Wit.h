/*	Raphael J. S. Costale
 *	Wit.h
 *	5/5/93
 */
#ifndef WIT_H
#define WIT_H
#include"HalfWit.h"
class Wit : public HalfWit
{
public:
// constructor
	Wit(Swamp& swamp);
// member funtions
	void draw(int);
	void setPos(Coord);
	
protected:
	Coord step();  // value returned by virtual function move
	
	List mPrevPath;
};
/*
 * implementation
 */
Wit::Wit(Swamp& swamp) : HalfWit(swamp) {}
void Wit::draw(int isSafe)
{
	cout << (isSafe ? 'W' : '*');
}
void Wit::setPos(Coord p)
{
	mCurrPath.purge(p);			// remove current position
	mPrevPath = mCurrPath;		// remember current path
	HalfWit::setPos(p);
}
Coord Wit::step()
{
	if (mPrevPath.length() > 0)		// follow previous path by popping first
	{					// element off of list
	mCurrPath.append(mPos);		// add current position to current path
	mPos = mPrevPath.remove(0); // remove the position in previous path
	}
	else
	mPos = HalfWit::step();		// act like a HalfWit
	return mPos;
}
#endif
// EOF
