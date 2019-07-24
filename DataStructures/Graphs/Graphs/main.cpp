#include <iostream>
#include <memory>
#include "RedBlackTree.h"
using namespace std;
using namespace dsa;

int main()
{
	RedBlackTree t(20);

	t.AddNode(30);
	t.AddNode(2);
	t.AddNode(4);
	t.AddNode(100);
	t.AddNode(24);
	
	for (auto i = t.BeginBFS(); !i.IsDone(); ++i)
	{
		cout << *i << endl;
	}

	return 0;
}