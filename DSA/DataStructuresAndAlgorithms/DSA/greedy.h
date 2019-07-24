#pragma once
#include <vector>
namespace dsa
{
	namespace greedy
	{
		std::vector<size_t> minStop(const std::vector<size_t> & stops, size_t distAB, size_t m);

		struct Activity
		{
			size_t start_time;
			size_t end_time;
		};
		bool operator<(const Activity& a, const Activity& b);

		std::vector<Activity> maxActivities(const std::vector<Activity>& activities);
	}
}