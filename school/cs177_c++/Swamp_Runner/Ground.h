/*	Raphael J. S. Costale
 *	Ground.h
 *	5/5/93
 */
#ifndef GROUND_H
#define GROUND_H
#include <ostream.h>
#include"SCell.h"

class Ground : public SCell
{
public:
// constructor
	Ground(char image);

// member funtions
	int safe();
};

//	implementation
Ground::Ground(char image = ' ') : SCell(image)
{
}
int Ground::safe()
{
	return 1;
}
#endif
// EOF
