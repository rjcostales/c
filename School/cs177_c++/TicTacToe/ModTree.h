/* Raphael J.S. Costales
 * ModTree.h 3/20/93
 *
 * ModTree is a modified binary tree.  It stores a sequence of moves in its
 * branches.  Each branch is made up of linked nodes.  Each node stores a X
 * move and O move.	 The class automatically determines the O move by picking
 * the next availible move not used previously.	 The branches grow by inputing
 * moves.  A branch is kept if it produces a winning or drawing sequence.
 * A branch is cutoff if it produces losing sequence.
 */
class ModTree
{
public:
	int getO();					// function to return mO
	void putX(int);				// function to store mX
	void learn() { curr = 0; }	// branch of tree good, keep it
	void forget();				// branch of tree bad, go back to root
	void save(char *);			// save tree to file
	void load(char *);			// load tree from file
	// constructor
	ModTree() { root = curr = 0; }
private:
	class Node
	{
	public:
		int mX; int mO;
		Node *next; Node *prev; Node *down;
		// constructor & destructor
		Node(int pX) { mX = pX; mO = 0; next = prev = down = 0; }
		~Node() { mX = mO = 0; next = prev = down = 0; }
	};
	Node *root, *curr;
	Node *addDown(int, Node *); // add branch down
	Node *addNext(int, Node *); // add branch next
	void cutNext(Node *);		// cut branch next
	void incrInt(Node *);		// increment mO
	int findInt(Node *);		// finds int not in branch
	int prevInt(int, Node *);	// returns true if int in branch
};
