#pragma once
#include <memory>
#include <vector>
#include <queue>
#include <stack>
// Data Structudes and Algorithms
namespace dsa
{
	using std::shared_ptr;
	using std::weak_ptr;
	using std::vector;
	using std::queue;

	class RedBlackTree
	{
	private:
		enum Color
		{
			Red, Black
		};

		class Node
		{
		public:
			Node(size_t d);
			Node(size_t d, Color c);

			size_t data;
			Color color;

			std::shared_ptr<Node> left;
			std::shared_ptr<Node> right;
			std::weak_ptr<Node> parent;

			// metadata for the Class-Owner
			bool visited;
		};

		void _Add_Node(shared_ptr<RedBlackTree::Node> current, shared_ptr<RedBlackTree::Node> P);
	public:
		class BFS_Iterator
		{
		public:
			BFS_Iterator(shared_ptr<Node> start);

			BFS_Iterator operator++(int);

			BFS_Iterator& operator++();

			bool IsDone();

			size_t operator*() const;

		private:
			shared_ptr<Node> start;
			shared_ptr<Node> current;
			std::queue<shared_ptr<Node>> q;
			bool isDone;
		};

		class DFS_Iterator
		{
		public:
			DFS_Iterator(shared_ptr<Node> start);

			DFS_Iterator& operator++();

			DFS_Iterator operator++(int);

			size_t operator*() const;

			bool IsDone() const;

			size_t Level() const;

		private:
			shared_ptr<Node> start;
			shared_ptr<Node> current;
			bool isDone;
			size_t level;
		};

		BFS_Iterator BeginBFS() const;

		DFS_Iterator BeginDFS() const;

		RedBlackTree();

		RedBlackTree(size_t data);

		~RedBlackTree();

		size_t GetRoot() const;

		long GetRootUseCount() const;

		void AddNode(size_t data);

	protected:

	private:
		shared_ptr<Node> root;
	};
}

