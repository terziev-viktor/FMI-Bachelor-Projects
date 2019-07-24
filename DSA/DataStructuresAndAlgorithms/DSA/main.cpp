#include <iostream>
#include <stdexcept>
#include <fstream>
#include "BSTree.h"
#include "dpa.h"
#include "greedy.h"
#include "Matrix.h"
#include "BinTreeNode.h"
#include <functional>
#include <sstream>
#include <future>
#include <thread>
#include "TabMngr.h"
#include <io.h>
#include <fcntl.h>
#include "TaskMngr.h"
#include "SetNode.h"
#include <unordered_map>
#include "ak47.h"

using namespace dpa;
using namespace dsa::greedy;
using namespace dsa;
using namespace std;
using namespace ak47;

// an object used to trasport exception across threads
static std::exception_ptr the_exc = nullptr;

double calc1(const double& num, std::promise<double> prmse = std::promise<double>())
{
	double result = 0.0;
	try
	{
		for (size_t i = 0; i < 20000 * 10000; i++)
		{
			result += num;
		}
		prmse.set_value(result);
		throw exception("calc1 threw an exception!");
	}
	catch (const std::exception&)
	{
		the_exc = current_exception();
	}
	return result;;
}


double calc2(double num, std::promise<double> prms = std::promise<double>())
{
	double d = 0.001;

	for (size_t i = 0; i < 15000 * 10000; i++)
	{
		num -= d;
		num /= 1.201;
	}

	prms.set_value(num);
	return num;
}

static std::exception_ptr exc_ptr = nullptr;
static std::condition_variable cv;
static std::mutex m;
static bool ready = false;

int main()
{
	Automata A("ab", 2, 0, { 1 });
	A(0, 'a', 1); //                ____
	A(0, 'b', 1); //           a,b  |  | a
	A(1, 'a', 1); //  start-(0)--->(1)<-
	A(1, 'b', 0); //         ^     |
				  //         |__b__|
				  //
						  //				___________a,b____________
	NAutomata AA = A + A; //                |___                      |___     
						  //           a,b  |  | a               a,b  |  | a
						  //  start-(0)---> 1 <-        start-(0)--->(1)<-
						  //         ^     |                   ^     |
						  //         |__b__|                   |__b__|
						  //						  //

	string str = "aaaaab";
	string str1 = "aabbaaabb";
	string str2 = "a";

	cout << "AA " << (AA.Matches(str) ? "matches " : "does not match ") << str << endl;
	cout << "AA " << (AA.Matches(str1) ? "matches " : "does not match ") << str1 << endl;
	cout << "AA " << (AA.Matches(str2) ? "matches " : "does not match ") << str2 << endl;

	return 0;
	// handling the exception thrown in f
	auto exc_handler = []() -> void
	{
		unique_lock<mutex> lck(m);
		cv.wait(lck, []{ return ready; });

		try
		{
			if (the_exc)
			{
				rethrow_exception(the_exc);
			}
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << endl;
		}

		lck.unlock();
	};

	// does some work but throws an exception
	auto f = []() -> void
	{
		try
		{
			this_thread::sleep_for(chrono::seconds(2));
			throw std::exception("f failed to exception");
		}
		catch (const std::exception&)
		{
			the_exc = current_exception();
		}
		unique_lock<mutex> lck(m);
		// changing ready
		ready = true;

		lck.unlock();
		cv.notify_one();
	};

	// does some work without exception
	auto g = []() -> void
	{
		this_thread::sleep_for(chrono::seconds(1));
	};

	thread t1(exc_handler);
	thread tf(f);
	thread tg(g);

	if (tf.joinable())
	{
		tf.join();
	}
	if (tg.joinable())
	{
		tg.join();
	}
	if (t1.joinable())
	{
		t1.join();
	}

	double result1 = 0.0;
	double result2 = 0.0;
	double calc1_arg = 1.2345;
	double calc2_arg = DBL_MAX;

	std::promise<double> calc1_promise;
	std::future<double> calc1_future = calc1_promise.get_future();
	std::thread(calc1, calc1_arg, std::move(calc1_promise)).detach();

	std::promise<double> calc2_promise;
	std::future<double> calc2_future = calc2_promise.get_future();
	std::thread(calc2, calc2_arg, std::move(calc2_promise)).detach();

	std::cout << "Calculating...\n";
	calc1_future.wait();
	calc2_future.wait();
	if (calc1_future.valid())
	{
		std::cout << calc1_future.get() << std::endl;
	}
	if (calc2_future.valid())
	{
		std::cout << calc2_future.get() << std::endl;
	}
	std::cout << "Done\n";
	try
	{
		if (the_exc)
		{
			std::rethrow_exception(the_exc);
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << endl;
	}

	std::shared_ptr<BinTreeNode<size_t>> root = std::make_shared<BinTreeNode<size_t>>(1u);
	root->Left = std::make_shared<BinTreeNode<size_t>>(20u);
	root->Right = std::make_shared<BinTreeNode<size_t>>(30u);
	root->Left->Parent = root;
	root->Right->Parent = root;

	std::cout << root->Left->Data << endl;
	std::cout << root->Right->Data << endl;
	shared_ptr<BinTreeNode<size_t>> left = root->Left;
	left->Left = make_shared<BinTreeNode<size_t>>(40u);
	left->Right = make_shared<BinTreeNode<size_t>>(50u);

	int g_val = 33;
	std::function<void(const size_t & val)> l = [&](const size_t& val) {
		std::cout << val << " g_val=" << g_val << endl;
	};

	std::cout << "root->BFS(l)\n";
	root->BFS(l);

	cout << endl;
	cout << "cutting rods: prices = { 1, 5, 8, 9, 10, 17, 17, 20, 24, 30 }\n";
	vector<int> r = cutRodProblem({ 1, 5, 8, 9, 10, 17, 17, 20, 24, 30 });
	cout << "r = {";
	for (size_t i = 1; i < r.size() - 1; ++i)
	{
		cout << r[i] << ',';
	}
	cout << r[r.size() - 1] << '}' << endl;
	vector<size_t> stops = minStop({ 1, 2, 3, 4, 5 }, 6, 1);

	cout << "greedy::minStop({ 1, 2, 3, 4, 5 }, 6, 1):\n";
	for (auto& i : stops)
	{
		cout << i << ' ';
	}
	vector<Activity> activities = maxActivities(
		{
			{0, 13},
			{2, 5},
			{4,6},
			{5, 9},
			{8, 14},
			{11, 12},
			{15, 20}
		});
	cout << "\nmaxActivities(..):\n";
	for (auto& i : activities)
	{
		cout << '[' << i.start_time << ':' << i.end_time << ']' << endl;
	}
	vector<size_t> L1 = { 7, 9, 3, 4, 8, 4 }, L2 = { 8, 5, 6, 4 ,5, 7 };
	vector<size_t> t12 = { 2, 3, 1, 3, 4 }, t21 = { 2, 1, 2, 2, 1 };
	size_t e1 = 2, e2 = 4;
	size_t x1 = 3, x2 = 2;
	try
	{
		size_t result = fastestRouteAssemblyLine(e1, L1, x1, e2, L2, x2, t12, t21);
		cout << "fastestRouteAssemblyLine(e1, L1, x1, e2, L2, x2, t12, t21):" << endl;
		cout << result << endl;
	}
	catch (const std::invalid_argument& e)
	{
		cerr << e.what() << endl;
		return 1;
	}

	return 0;
}

