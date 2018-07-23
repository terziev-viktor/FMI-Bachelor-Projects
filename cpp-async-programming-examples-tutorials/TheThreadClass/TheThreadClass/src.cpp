#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <future>
#include <condition_variable>
#include <Windows.h>
using namespace std;

static mutex v_mutex; // mutex used to lock the numbers vector
static condition_variable cv;
static vector<int> numbers;
static bool added_even = false; // the first number to be added should be 0 (an even number)

int complicated_calculation(promise<int> p)
{
	int r = rand() % 1000 + 1;
	int rand_hard_work_time = rand() % 2000 + 300;

	//cout << "Hard work in functuon" << endl;
	Sleep(rand_hard_work_time); // simulating hark work
	//cout << "Done hard work in functuon" << endl;
	p.set_value(r); // setting value in the primise so we can retrieve it with .get() method of promise's future object
	return r; // returning the result like in a normal function so we can use it without the promise if we want
}

// adding even numbers to global vector
void add_evens(int n)
{
	for (int i = 0; i < n; i++)
	{
		if (i % 2 == 0)
		{
			unique_lock<mutex> evens_lock(v_mutex); // taking ownership of the mutex
			cv.wait(evens_lock, [] {return !added_even; }); // waiting for the condition variable - odd number to be added
			numbers.push_back(i);
			Sleep(40); // simulate hard work while owning the mutex
			added_even = true; // modifying the condition variable
			evens_lock.unlock(); // releasing the mutex
			cv.notify_one(); // notifying other thread
		}
	}
}

void add_odds(int n)
{
	for (int i = 0; i < n; i++)
	{
		if (i % 2)
		{
			unique_lock<mutex> odds_lock(v_mutex); // taking ownership of the mutex
			cv.wait(odds_lock, [] {return added_even; }); // waiting for the condition variable - even number to be added
			numbers.push_back(i);
			added_even = false; // modifying the condition variable
			odds_lock.unlock(); // releasing the mutex
			cv.notify_one(); // notifying other thread
		}
	}
}
// printing the vector
ostream & operator<<(ostream & os, const vector<int> numbers)
{
	for (size_t i = 0; i < numbers.size(); i++)
	{
		if (numbers[i] != i)
		{
			os << "Wrong number on position " << i << " - " << numbers[i];
			return os;
		}
	}
	os << '(';
	for (size_t i = 0; i < numbers.size() - 1; i++)
	{
		os << numbers[i] << ", ";
	}
	os << numbers[numbers.size() - 1] << ')';
	return os;
}

// read this https://en.cppreference.com/w/cpp/thread/thread for the thread class
// https://en.cppreference.com/w/cpp/thread/condition_variable for condition_variable
// https://en.cppreference.com/w/cpp/thread/mutex for mutex

int main()
{
	srand(time(NULL));
	promise<int> promise_calc_1, promise_calc_2;
	future<int> future_calc_1 = promise_calc_1.get_future();
	future<int> future_calc_2 = promise_calc_2.get_future();
	thread calc_thread_1(complicated_calculation, move(promise_calc_1)); // starting the hard work
	thread calc_thread_2(complicated_calculation, move(promise_calc_2)); // starting another hard work
	thread adding_even_numbers_thread(add_evens, 30);
	thread adding_odd_numbers_thread(add_odds, 30);

	bool r1 = false, r2 = false;
	while (!r1 || !r2) // waiting untill both hard calculatuins are ready
	{
		if (!r1 && future_calc_1._Is_ready())
		{
			int result_1 = future_calc_1.get();
			cout << "result 1 is ready: " << result_1 << endl;
			r1 = true;
		}
		if (!r2 && future_calc_2._Is_ready())
		{
			int result_2 = future_calc_2.get();
			cout << "result 2 is ready: " << result_2 << endl;
			r2 = true;
		}
	}
	//numbers[3] = 10; // simulating wrong result

	// joining all threads
	calc_thread_1.join();
	calc_thread_2.join();
	adding_even_numbers_thread.join();
	adding_odd_numbers_thread.join();
	cout << numbers << endl;
	return 0;
}