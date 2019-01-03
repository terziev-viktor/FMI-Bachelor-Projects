#pragma once

template<typename T, class Comparator>
class BinTree 
{
protected:
	class Node
	{
	private:
		T data;
		Node * left;
		Node * right;
	public:
		Node(const T & data)
			:data(data), left(nullptr),right(nullptr)
		{

		}

		~Node()
		{
			if (left)
			{
				delete left;
			}
			if (right)
			{
				delete right;
			}
		}

		T & Data() { return this->data; }

		Node * & Left() { return this->left; }

		Node * & Right() { return this->right; }
	};

	Node * root;

	Comparator compare;

	Node * Find() const
	{
		if (!this->root)
		{
			return false;
		}
		Node * c = this->root;
		while (c->data != element)
		{
			if (compare(element, c->data))
			{
				c = c->left;
			}
			else
			{
				c = c->right;
			}
			if (c == nullptr)
			{
				return c;
			}
		}
		return c;
	}

public:
	class NodeProxy
	{
	private:
		Node * node;
	public:
		NodeProxy(Node * n)
		{
			if (n==nullptr)
			{
				throw std::exception("No such node exists");
			}
			this->node = n;
		}
		const T & GetData() const { return n->Data(); }
		T & GetData() { return n->Data(); }
		NodeProxy GetLeft() const { return NodeProxy(node->Left()); }
		NodeProxy GetRight() const { return NodeProxy(node->Right()); }
	};

	BinTree()
	:root(nullptr)
	{

	}

	BinTree(const T & data)
	{
		this->root = new Node(data);
	}

	~BinTree()
	{
		delete this->root;
	}

	NodeProxy Root() const
	{
		return NodeProxy(this->root);
	}

	void Insert(const T & value)
	{
		if (!this->root)
		{
			this->root = new Node(value);
		}
		else
		{
			Node * & c = this->root;
			while (c != nullptr)
			{
				if (compare(value, c->Data())) // go left
				{
					c = c->Left();
				}
				else
				{
					c = c->Right();
				}
			}
			c = new Node(value);
		}
	}

	bool Contains(const T & element)
	{
		return this->Find(element) != nullptr;
	}

	NodeProxy Get(const T & element)
	{
		return NodeProxy(this->Find(element));
	}
};