#include "greedy.h"
#include <algorithm>
std::vector<size_t> dsa::greedy::minStop(const std::vector<size_t> & stops, size_t distAB, size_t m)
{
	std::vector<size_t> result;
	size_t left = m;
	size_t lastStop = 0;
	for (size_t i = 0; i < stops.size() - 1; ++i)
	{
		size_t dist = stops[i] - lastStop;
		lastStop = stops[i];
		left -= dist;
		if (left < stops[i + 1] - stops[i])
		{
			result.push_back(i);
			left = m;
		}
	}
	if (left < distAB - lastStop)
	{
		result.push_back(stops.size() - 1);
	}
	
	return result;
}

bool dsa::greedy::operator<(const Activity& a, const Activity& b)
{
	return a.end_time < b.end_time;
}

using dsa::greedy::Activity;
std::vector<dsa::greedy::Activity> dsa::greedy::maxActivities(const std::vector<Activity>& inp)
{
	std::vector<Activity> result;
	std::vector<Activity> activities = inp;
	std::sort(activities.begin(), activities.end());

	result.push_back(activities[0]);
	Activity & last = activities[0];
	for (size_t i = 1; i < activities.size(); i++)
	{
		if (last.end_time <= activities[i].start_time)
		{
			result.push_back(activities[i]);
			last = activities[i];
		}
	}

	return result;
}
