#include "RedBlackTree.h"
#include <iostream>

namespace dsa
{
	RedBlackTree::BFS_Iterator RedBlackTree::BeginBFS() const
	{
		return BFS_Iterator(this->root);
	}

	RedBlackTree::DFS_Iterator RedBlackTree::BeginDFS() const
	{
		return DFS_Iterator(this->root);
	}

	RedBlackTree::RedBlackTree()
	{

	}

	RedBlackTree::RedBlackTree(size_t root_data)
	{
		this->root = std::make_shared<Node>(root_data, Color::Black);
	}

	RedBlackTree::~RedBlackTree()
	{
		//std::cout << "~RedBlackTree()" << std::endl;
	}

	size_t RedBlackTree::GetRoot() const
	{
		return this->root->data;
	}

	long RedBlackTree::GetRootUseCount() const
	{
		return this->root.use_count();
	}

	void RedBlackTree::_Add_Node(shared_ptr<RedBlackTree::Node> current, shared_ptr<RedBlackTree::Node> P)
	{
		shared_ptr<RedBlackTree::Node>* p_current = &current;

		while (*p_current)
		{
			if ((*p_current)->data < P->data)
			{
				p_current = &(*p_current)->left;
			}
			else
			{
				p_current = &(*p_current)->right;
			}
		}
		(*p_current) = P;
	}

	void RedBlackTree::AddNode(size_t data)
	{
		shared_ptr<Node> n = std::make_shared<Node>(data);
		_Add_Node(this->root, n);
	}

	RedBlackTree::Node::Node(size_t d)
	{
		data = d;
		color = Color::Black;
		visited = false;
	}

	RedBlackTree::Node::Node(size_t d, Color c)
	{
		data = d;
		color = c;
		visited = false;
	}

	// Pass the root of your tree
	RedBlackTree::BFS_Iterator::BFS_Iterator(shared_ptr<Node> start)
	{
		if (!start)
		{
			this->isDone = true;
		}
		else
		{
			this->isDone = false;
			this->start = start;
			this->current = start;
			if (this->current->left)
			{
				this->q.push(this->current->left);
			}
			if (this->current->right)
			{
				this->q.push(this->current->right);
			}
		}
	}

	RedBlackTree::BFS_Iterator RedBlackTree::BFS_Iterator::operator++(int)
	{
		RedBlackTree::BFS_Iterator old(*this);
		if (this->q.empty())
		{
			this->isDone = true;
		}
		else
		{
			this->current = this->q.front();
			if (this->current->left)
			{
				this->q.push(this->current->left);
			}
			if (this->current->right)
			{
				this->q.push(this->current->right);
			}
			this->q.pop();
		}
		return old;
		
	}

	RedBlackTree::BFS_Iterator& RedBlackTree::BFS_Iterator::operator++()
	{
		if (this->q.empty())
		{
			this->isDone = true;
		}
		else
		{
			this->current = this->q.front();
			if (this->current->left)
			{
				this->q.push(this->current->left);
			}
			if (this->current->right)
			{
				this->q.push(this->current->right);
			}
			this->q.pop();
			
		}
		return *this;
	}

	bool RedBlackTree::BFS_Iterator::IsDone()
	{
		return this->isDone;
	}

	size_t RedBlackTree::BFS_Iterator::operator*() const
	{
		return this->current->data;
	}

	RedBlackTree::DFS_Iterator::DFS_Iterator(shared_ptr<Node> start)
	{
		this->start = start;
		this->level = 0;
		this->isDone = false;
		this->current = start;

		while (this->current->left)
		{
			this->current = this->current->left;
		}
	}

	RedBlackTree::DFS_Iterator& RedBlackTree::DFS_Iterator::operator++()
	{
		return *this;
		// TODO: insert return statement here
	}

	RedBlackTree::DFS_Iterator RedBlackTree::DFS_Iterator::operator++(int)
	{
		DFS_Iterator old(*this);
		
		return old;
	}

	size_t RedBlackTree::DFS_Iterator::operator*() const
	{
		return this->current->data;
	}

	bool RedBlackTree::DFS_Iterator::IsDone() const
	{
		return this->isDone;
	}

	size_t RedBlackTree::DFS_Iterator::Level() const
	{
		return this->level;
	}
}