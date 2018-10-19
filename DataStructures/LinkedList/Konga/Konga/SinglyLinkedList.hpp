#pragma once

template<typename T>
class SinglyLinkedList
{
public:
	SinglyLinkedList()
	{
		this->_root = nullptr;
	}
	~SinglyLinkedList()
	{
		if (this->_root)
		{
			delete this->_root;
		}
	}
	SinglyLinkedList & Add(const T & value)
	{
		Node * n = new Node();
		n->setData(value);
		n->setNext(this->_root);
		this->_root = n;
		return *this;
	}
	T getRoot() const
	{
		return this->_root->getData();
	}
	T getAt(size_t index) const
	{
		Node * tmp = this->_root;
		for (size_t i = 0; i < index; ++i)
		{
			if (tmp->getNext() == nullptr)
			{
				throw "Invalid Index";
			}
			tmp = tmp->getNext();
		}
		return tmp->getData();
	}
protected:
	class Node
	{
	public:
		Node()
			:_next(nullptr)
		{

		}
		Node(const T & value)
		{
			this->setValue(value);
			this->_next = nullptr;
		}
		~Node()
		{
			if (this->_next)
			{
				delete this->_next;
			}
		}

		void setData(const T & value)
		{
			this->_data = value;
		}
		T getData() const
		{
			return this->_data;
		}
		void setNext(Node * next)
		{
			if (this == next)
			{
				throw "no logic exception";
			}
			this->_next = next;
		}
		Node * getNext() const
		{
			return this->_next;
		}
	private:
		Node * _next;
		T _data;
	};

private:
	Node * _root;
};
