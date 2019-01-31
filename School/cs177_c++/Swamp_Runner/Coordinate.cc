/*	Raphael J. S. Costale
 *	Coordinate.cc
 *	5/5/93 */
#ifndef COORD_CC#define COORD_CC
#include"Coordinate.h"

// Friend operator
ostream& operator<<(ostream& pOstream, const Coord& pC)
{
    pOstream << '(' <<	pC.x <<", " << pC.y << ')';
    return pOstream;
}

// Coordinate operators
void Coord::operator=(Coord& pC)
{
    x = pC.x;
    y = pC.y;
}

Coord Coord::operator+(Coord& pC)
{
    Coord temp;
    temp.x = x + pC.x;
    temp.y = y + pC.y;
    return temp;
}

int Coord::operator==(Coord& pC) const
{
    return (x == pC.x && y == pC.y);
}

int Coord::operator!=(Coord& pC) const
{
    return (x != pC.x || y != pC.y);
}

// Type Conversions
Coord::Coord(int pX, int pY )
{
    x = pX;
    y = pY;
}

Coord::Coord()
{
    x = 0;
    y = 0;
}

Coord::Coord(Coord &pC)
{
    x = pC.x;
    y = pC.y;
}

#endif
