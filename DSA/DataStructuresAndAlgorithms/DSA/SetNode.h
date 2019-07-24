#pragma once

// A node used for a Union-Find data structure
class SetNode
{
private:
	SetNode* parent;
public:
	size_t data;
	SetNode():data(0u),parent(nullptr)
	{

	}
	SetNode(int d):data(d),parent(nullptr){}
	SetNode(int d, SetNode * p):data(d),parent(p){}

	void setParent(SetNode* parent);

	SetNode* getParent() const;

	bool isInAnySet() const;

	bool isInSameSetAs(const SetNode& other) const;

	void MakeSet();

	void Union(SetNode* with);
};