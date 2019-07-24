#include "BSTree.h"

namespace dsa
{
	BSTree::BSTree(size_t root_data)
	{
		this->root = std::make_shared<_Node>(root_data);
		
	}

	BSTree::_Node::_Node(size_t root_data)
	{
		this->data = root_data;
		this->visited = false;
		this->color = Color::White;
	}
}