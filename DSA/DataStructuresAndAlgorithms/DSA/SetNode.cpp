#include "SetNode.h"

inline void SetNode::setParent(SetNode* parent)
{
	this->parent = parent;
}
inline SetNode* SetNode::getParent() const
{
	return this->parent;
}
inline bool SetNode::isInAnySet() const
{
	return this->parent != nullptr;
}

bool SetNode::isInSameSetAs(const SetNode& other) const
{
	const SetNode* a = this;
	const SetNode* b = &other;

	while (a->parent != a) a = a->parent;
	while (b->parent != b) b = b->parent;

	return a == b;
}

inline void SetNode::MakeSet()
{
	this->parent = this;
}

void SetNode::Union(SetNode* with)
{
	SetNode* a = this;
	SetNode* b = with;
	size_t a_count = 0, b_count = 0;

	while (a->parent != a)
	{
		a = a->parent;
		++a_count;

	}
	while (b->parent != b)
	{
		b = b->parent;
		++b_count;
	}
	if (a_count > b_count)
	{
		a->parent = b->parent;
	}
	else
	{
		b->parent = a->parent;
	}
}
