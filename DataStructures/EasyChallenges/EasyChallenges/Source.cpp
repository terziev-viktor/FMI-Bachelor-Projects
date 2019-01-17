#include <iostream>
using std::cout;
using std::endl;

void placeInTree(size_t indexBegin, size_t indexEnd, size_t root, int * FBT, int * RON, size_t size)
{
	if (indexEnd == indexBegin)
	{
		return;
	}
	size_t i = indexBegin + (indexEnd - indexBegin) / 2;
	FBT[root] = RON[i];
	size_t leftChild = 2 * root + 1;
	size_t rightChild = 2 * root + 2;

	if (leftChild < size)
	{
		placeInTree(indexBegin, i, leftChild, FBT, RON, size);
	}
	if (rightChild < size)
	{
		placeInTree(i + 1, indexEnd, rightChild, FBT, RON, size);
	}
}

void fillBinTree(int * fullBinTree, int * rowOfNums, size_t size)
{
	size_t mid = size / 2;

	placeInTree(0, size, 0, fullBinTree, rowOfNums, size);
}

int main()
{
	const size_t size = 15;
	int rowOfNums[size] = { 7, 11, 23, 48, 53, 119, 219, 319, 419, 519, 619, 719, 819, 919, 1019 };

	int fullBinTree[size];
	for (size_t i = 0; i < size; ++i)
	{
		fullBinTree[i] = 0;
	}
	fillBinTree(fullBinTree, rowOfNums, size);

	for (size_t i = 0; i < size; ++i)
	{
		cout << fullBinTree[i] << ' ';
	}
	cout << endl;
	return 0;
}