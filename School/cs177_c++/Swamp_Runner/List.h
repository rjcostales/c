/*	Raphael J. S. Costale
 *	List.h
 *	5/5/93
 *
 *	List class with Coorinate elements
 */
#ifndef LIST_H
#define LIST_H
#include <stdlib.h>
#include <ostream.h>
#include"Coordinate.h"

typedef Coord Type;
class List
{
	friend ostream& operator<<(ostream&, const List&);
public:
// constructors & destructors
	List() { mHead = 0; mSize = 0; }
	List(List&);
	~List();
// operator and member funtions
	List operator+(List&);
	List& operator=(List&);
	Type& operator[](int);
	const Type& operator[](int) const;
	int operator==(List&) const;
	int operator!=(List&) const;

	int length() const;
	int isMember(Type) const;
	void clear();
	List& insert(Type, int pIndex = 0);
	List& append(Type);
	Type remove(int pIndex = 0);
	List& purge(Type);

private:
	int mSize;
	class Element
	{
	public:
		Element(Type pData) { mData = pData; mNext = 0; };
		Type mData;
		Element *mNext;
	};
	Element *mHead;
	Type& atIndex(int) const;
	void error(char *) const;
};
#endif
