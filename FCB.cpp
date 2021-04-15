#include "FCB.h"

FCB::FCB(char newtype, string newname, FCB* newparent, FCB* newchild, FCB* newsibling)
{
	name = newname;
	type = newtype;
	parent = newparent;
	child = newchild;
	sibling = newsibling;
}
