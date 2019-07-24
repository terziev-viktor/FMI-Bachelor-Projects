#pragma once
#include <memory>
#include <functional>
namespace dsa
{
	using std::shared_ptr;
	using std::weak_ptr;

	template<typename TData>
	class BinTreeNode
	{
	public:
		BinTreeNode()
		{

		}
		BinTreeNode(const TData& data)
		{
			this->Data = data;
		}
		void BFS(std::function<void(const TData& param)>& lambda)
		{
			lambda(this->Data);
			if (this->Left)
			{
				m_BFS(this->Left, lambda);
			}
			if (this->Right)
			{
				m_BFS(this->Right, lambda);
			}
		}

		TData Data;
		weak_ptr<BinTreeNode> Parent;
		shared_ptr<BinTreeNode> Left;
		shared_ptr<BinTreeNode> Right;

	private:
		void m_BFS(shared_ptr<BinTreeNode>& n, std::function<void(const TData& param)>& lambda)
		{
			if (n)
			{
				lambda(n->Data);
				if (n->Left)
				{
					m_BFS(n->Left, lambda);
				}
				if (n->Right)
				{
					m_BFS(n->Right, lambda);
				}
			}
		}
	};
}
