/*	Raphael J. S. Costale
 *	SCell.h - Abstract Base Class
 *	5/5/93
 */
#ifndef SCELL_H
#define SCELL_H
#include <ostream.h>
#include <iostream.h>

class SCell
{
public:
// Constructor
	SCell(char image) { mImage = image; }

	virtual int safe() = 0;	 // returns true is cell can be stood on

	void draw() { cout << mImage; }

protected:
	char mImage;
};
#endif
// EOF
