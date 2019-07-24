#pragma once

#include <vector>
#include <thread>
#include <future>

class TaskMngr
{
public:
	std::vector<std::packaged_task<void()>> Tasks;

	void InvokeAll()
	{
		for (auto& i : Tasks)
		{
			i();
		}
	}

protected:
};

