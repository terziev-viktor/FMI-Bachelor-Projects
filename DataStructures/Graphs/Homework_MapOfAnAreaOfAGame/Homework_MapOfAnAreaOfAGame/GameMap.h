#pragma once
#include <string>
#include <vector>
#include <unordered_map>
using std::string;
using std::vector;
using std::pair;
using std::unordered_map;

struct Area
{
	Area()
		:visiting(false), visited(false)
	{}

	Area(const string & name)
		:name(name), visiting(false), visited(false)
	{}

	string name;

	// the area is being visited during the traversal
	bool visiting;

	// the area has been visited during the traversal
	bool visited;

	vector<pair<string, bool>> keys; // (keyname, keyExists)
};

struct AreaLink
{
	string unlockedWithKey; // key needed to get through this link

	Area * toArea;
};

typedef unordered_map<string, Area> Areas;

typedef unordered_map<string, vector<AreaLink>> AreaLinks;
