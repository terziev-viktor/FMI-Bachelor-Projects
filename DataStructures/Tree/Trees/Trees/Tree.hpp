#pragma once
#include <ostream>
#include <queue>
using std::queue;
using std::ostream;

// Linked binary tree. Removing elements without copying data.
template<typename T, typename Comparator>
class Tree
{
public:
	struct Leaf
	{
		T data;
		Leaf * left;
		Leaf * right;
	};

	Comparator compare;

private:
	Leaf * root;

	Leaf * * max(Leaf * * r)
	{
		if (*r == nullptr)
		{
			return r;
		}
		while ((*r)->right != nullptr)
		{
			r = &(*r)->right;
		}
		return r;
	}

	void del_Tree();

	void del(Leaf * p);

public:
	Tree(const T&root);

	Tree();

	~Tree();

	void insert(const T & item);

	void remove(const T & item);

	void print(ostream & out) const;
};

template<typename T, typename Comparator>
inline void Tree<T, Comparator>::del_Tree()
{
	Leaf * p = this->root;
	this->del(p);
}

template<typename T, typename Comparator>
inline void Tree<T, Comparator>::del(Leaf * p)
{
	if (p->left)
	{
		this->del(p->left);
	}
	if (p->right)
	{
		this->del(p->right);
	}
	delete p;
}

template<typename T, typename Comparator>
inline Tree<T, Comparator>::Tree(const T & root)
{
	this->root = new Leaf();
	this->root->data = root;
	this->root->left = nullptr;
	this->root->right = nullptr;
}

template<typename T, typename Comparator>
inline Tree<T, Comparator>::Tree()
{
	this->root = nullptr;
}

template<typename T, typename Comparator>
inline Tree<T, Comparator>::~Tree()
{
	if (this->root != nullptr)
	{
		this->del_Tree();
	}
	
}

template<typename T, typename Comparator>
inline void Tree<T, Comparator>::insert(const T & item)
{
	if (this->root == nullptr)
	{
		this->root = new Leaf();
		this->root->data = item;
		this->root->left = this->root->right = nullptr;
	}
	else
	{
		Leaf * * l = &this->root;
		while (*l != nullptr)
		{
			if (compare((*l)->data, item) > 0)
			{
				l = &(*l)->left;
			}
			else
			{
				l = &(*l)->right;
			}
		}
		*l = new Leaf();
		(*l)->data = item;
		(*l)->left = nullptr;
		(*l)->right = nullptr;
	}
}

template<typename T, typename Comparator>
inline void Tree<T, Comparator>::remove(const T & item)
{
	Leaf * p; // will remove this
	Leaf * * parent; // pointer to parent of p;
	parent = nullptr;
	p = this->root;
	bool isLeft = false;
	bool found = false;
	while (p)
	{
		int cmp = compare(item, p->data);
		if (cmp < 0)
		{
			parent = &p;
			p = p->left;
			isLeft = true;
		}
		else if (cmp > 0)
		{
			parent = &p;
			p = p->right;
			isLeft = false;
		}
		else
		{
			found = true;
			break;
		}
	}

	if (!found)
	{
		throw "Element not found";
	}

	if (parent != nullptr && p->right == nullptr && p->left == nullptr)
	{
		if (isLeft)
		{
			(*parent)->left = nullptr;
		}
		else
		{
			(*parent)->right = nullptr;
		}
		delete p;
	}
	else if (parent != nullptr && p->right != nullptr && p->left == nullptr)
	{
		if (isLeft)
		{
			(*parent)->left = p->right;
		}
		else
		{
			(*parent)->right = p->right;
		}
		delete p;
	}
	else if (parent != nullptr && p->left != nullptr && p->right == nullptr)
	{
		if (isLeft)
		{
			(*parent)->left = p->left;
		}
		else
		{
			(*parent)->right = p->left;
		}
		delete p;
	}
	else
	{
		Leaf * * p_inorder = &p->left;
		p_inorder = max(p_inorder);
		if (parent != nullptr)
		{
			if (isLeft)
			{
				(*parent)->left = *p_inorder;
			}
			else
			{
				(*parent)->right = *p_inorder;
			}
		}
		else // removing the root
		{
			this->root = *p_inorder;
		}
		if (*p_inorder != nullptr)
		{
			(*p_inorder)->left = p->left != *p_inorder ? p->left : nullptr;
			(*p_inorder)->right = p->right != *p_inorder ? p->right : nullptr;
			*p_inorder = nullptr;
		}
		delete p;
	}

}

template<typename T, typename Comparator>
inline void Tree<T, Comparator>::print(ostream & out) const
{
	if (this->root == nullptr)
	{
		out << "Empty";
		return;
	}
	queue<const Leaf*> q;
	q.push(this->root);
	while (!q.empty())
	{
		const Leaf * p = q.front();
		out << (p->left == nullptr ? 0 : p->left->data) << " <-- ";
		out << p->data << " --> " << (p->right == nullptr ? 0 : p->right->data) << '\n';
		if (p->right)
		{
			q.push(p->right);
		}
		if (p->left)
		{
			q.push(p->left);
		}
		q.pop();
	}
}
