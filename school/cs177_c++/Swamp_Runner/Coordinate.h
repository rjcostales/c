/*	Raphael J. S. Costale
 *	Coordinate.h - Data Type Class
 *	5/5/93
 *
 *	Coordinate class consisting of (x, y) members
 */
#ifndef COORD_H
#define COORD_H
#include <iostream.h>

class Coord
{
	friend ostream& operator<<(ostream&, const Coord&);

public:
// constructors
	Coord(int, int);
	Coord();
	Coord(Coord&);

// operators
	void operator=(Coord&);
	Coord operator+(Coord&);
	int operator==(Coord&) const;
	int operator!=(Coord&) const;

// data members
	int x;	 // made x & y public for ease
	int y;	 // and more readable code.
};

#endif

// EOF
