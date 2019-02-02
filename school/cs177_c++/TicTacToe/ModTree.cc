/* Raphael J.S. Costales
 * ModTree.cc 3/20/93
 */

#include <iostream>
#include <fstream>
#include <ctype.h>
#include "ModTree.h"

int ModTree::getO()
{
	if (curr->mO)
		return (curr->mO);

	return findInt(curr);				// if mO O find next int
}

void ModTree::putX(int pX)
{
	if (curr)
		curr = addNext(pX, curr);
	else								// curr is 0 at the beginning of
		curr = addDown(pX, root);		// of each game
}

void ModTree::forget()
{
	if (curr->prev)
		incrInt(curr->prev);			// create a new branch
	curr = 0;
}

ModTree::Node *ModTree::addDown(int pX, Node *node)
{
	if (!root)							// newOB tree
	{
		root = new ModTree::Node(pX);
		return root;
	}

	while (node->down)
	{
		if (node->mX == pX)				// if node already exist
			return node;				// return pointer to node
		node = node->down;
	}

	if (node->mX == pX)
		return node;

	node->down = new ModTree::Node(pX);// create and return pointer to
	node->down->prev = node->prev;	   // new node
	return node->down;
}

ModTree::Node *ModTree::addNext(int pX, Node *node)
{
	if (!node->next)					// look next first
	{
		node->next = new ModTree::Node(pX);
		node->next->prev = node;
		return node->next;
	}

	return addDown(pX, node->next);		// then look down
}

void ModTree::cutNext(Node *node)
{
	if (node->down)						// delete all nodes in a branch
		cutNext(node->down);			// using post order transversing
	if (node->next)
		cutNext(node->next);

	if (node->prev)
		node->prev->next = 0;
	delete node;
}

void ModTree::incrInt(Node *node)
{
	if (findInt(node) > 9)				// if a learn int can't be found at node
		incrInt(node->prev);			// then increment previous node
	else								// if mO value changes, then the branch
		cutNext(node->next);			// pointed to by next is cut off
}

int ModTree::findInt(Node *node)
{
	while (prevInt(++(node->mO), node));
	return node->mO;
}

int ModTree::prevInt(int i, Node  *node)
{
	if (node->mX == i)
		return 1;

	while (node = node->prev)
		if ((node->mO == i)||(node->mX == i))
			return 1;

	return 0;
}

void ModTree::save(char *filename)		// a crude function, but it works!
{
	ifstream inFile(filename, ios::in);
	char c;

	if (inFile) // some file checking
	{
		cout << "Overwrite existing file:" << filename << " [y/n] ";
		cin >> c;
		cout << '\n';
		if (tolower(c) != 'y')
			return;
	}

	ofstream outFile(filename, ios::out);
	Node  *node1, *node2, *node3, *node4, *node5;

	node1 = root;
	while (node1)
	{
		if (node2 = node1->next) while (node2)
		{
			if (node3 = node2->next) while (node3)
			{
				if (node4 = node3->next) while (node4)
				{
					if (node5 = node4->next) while (node5)
					{
						outFile << node1->mX << node1->mO;
						outFile << node2->mX << node2->mO;
						outFile << node3->mX << node3->mO;
						outFile << node4->mX << node4->mO;
						outFile << node5->mX << node5->mO << '\n';
						node5 = node5->down;
					}
					else
					{
						outFile << node1->mX << node1->mO;
						outFile << node2->mX << node2->mO;
						outFile << node3->mX << node3->mO;
						outFile << node4->mX << node4->mO << '\n';
					}
					node4 = node4->down;
				}
				else
				{
					outFile << node1->mX << node1->mO;
					outFile << node2->mX << node2->mO;
					outFile << node3->mX << node3->mO << '\n';
				}
				node3 = node3->down;
			}
			else
			{
				outFile << node1->mX << node1->mO;
				outFile << node2->mX << node2->mO << '\n';
			}
			node2 = node2->down;
		}
		else
			outFile << node1->mX << node1->mO << '\n';
		node1 = node1->down;
	}
}

void ModTree::load(char *filename)
{
	ifstream inFile(filename, ios::in);
	char c;								// mX & mO read in as char '0' - '9'
	int i = 0;							// flag to tell if mX or xO value

	if (!inFile)						// some file checking
	{
		cout << "file:" << filename << " doesn't exist, no data loaded!\n";
		return;
	}

	learn();

	while (inFile.get(c))
	{
		if (c == '\n')
			learn();
		else
		{
			if (i = !i)
				putX(c - '0');
			else
				curr->mO = c - '0';
		}
	}
	cout << "file:" << filename << " loaded.\n";
}

// EOF
