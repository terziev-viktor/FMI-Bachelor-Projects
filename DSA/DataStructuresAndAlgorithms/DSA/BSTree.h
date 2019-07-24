#pragma once
#include <memory>
namespace dsa
{
	using std::shared_ptr;
	using std::weak_ptr;

	class Node;

	class _Node;

	typedef shared_ptr<Node> NodePtr;

	class BSTree
	{
	public:
		class Node
		{
		public:
			Node(const _Node& other);
			size_t data;
		};

		NodePtr Root() const;

		BSTree(size_t root);

	private:
		enum Color
		{
			White, Grey
		};
		class _Node
		{
		public:
			_Node(size_t root_data);

			size_t data;
			shared_ptr<_Node> left;
			shared_ptr<_Node> right;

			bool visited;
			Color color;

		};

		shared_ptr<_Node> root;

	};
}
