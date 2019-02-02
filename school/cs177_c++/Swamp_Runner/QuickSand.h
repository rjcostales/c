/*	Raphael J. S. Costale
 *	QuickSand.h
 *	5/5/93
 */
#ifndef QSAND_H
#define QSAND_H
#include"SCell.h"

class QSand : public SCell
{
public:
// constructor
	QSand(char image);

// member funtions
	int safe();
};
//	implementation
QSand::QSand(char image = '+') : SCell(image)
{
}
int QSand::safe()
{
	return 0;
}
#endif
// EOF
