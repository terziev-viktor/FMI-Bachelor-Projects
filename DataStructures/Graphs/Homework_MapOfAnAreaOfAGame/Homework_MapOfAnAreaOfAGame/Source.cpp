#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
using namespace std;

struct Area
{
	Area()
		:visiting(false), visited(false) 
	{}

	Area(const string & name) 
		:name(name),visiting(false),visited(false)
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

typedef unordered_map<string, vector<AreaLink>> GameMap;

bool inp(GameMap & gameMap, Areas & areas, const string & path)
{
	ifstream in(path);
	if (!in)
	{
		cerr << "Could not open " << path;
		return false;
	}

	string str;
	string areaTo;
	string areaFrom;
	size_t arrowIndex = 0;
	size_t bracketIndex = 0;
	const Areas::iterator & end = areas.end();

	getline(in, str); // reading [zones]
	bool readingZones = true;
	while (in)
	{
		if (readingZones)
		{
			getline(in, str);
			if (str.size() < 2)
			{
				readingZones = false;
				getline(in, str); // reading [keys]
				continue;
			}

			arrowIndex = str.find("->");
			areaFrom = str.substr(0, arrowIndex - 1);
			bracketIndex = str.find_first_of('[');
			if (bracketIndex > str.size())
			{
				bracketIndex = str.size();
			}

			arrowIndex += 3;
			areaTo = str.substr(arrowIndex, (bracketIndex - (arrowIndex)) - 1);

			// no object has been created
			if (areas.find(areaFrom) == end)
			{
				areas.insert(
					{
						areaFrom,
						Area(areaFrom)
					});
			}

			// no object has been created
			if (areas.find(areaTo) == end)
			{
				areas.insert(
					{
						areaTo,
						Area(areaTo)
					});
			}

			string unlockedWithKey = "";
			if (bracketIndex < str.size())
			{
				unlockedWithKey = str.substr(bracketIndex + 1, str.size() - bracketIndex - 3);
			}

			gameMap[areaFrom].push_back(AreaLink() = {
				unlockedWithKey,
				&areas[areaTo]
				}); // add a link from areaFrom to areaTo with key unlockedWithKey
		}
		else // reading keys
		{
			getline(in, str); // some key -> some area
			if (!in || str.empty())
			{
				return true;
			}
			arrowIndex = str.find("->");
			areaFrom = str.substr(0, arrowIndex - 1); // some key
			arrowIndex += 3;
			areaTo = str.substr(arrowIndex, (str.size() - (arrowIndex)) - 1); // some area
			areas[areaTo].keys.push_back(pair<string, bool>(areaFrom, true));
		}
	}
	return true;
}

struct TraversalParams
{
	GameMap & gameMap;

	Areas & areas;

	Area & currentArea;

	vector<AreaLink> & currentAreaLinks;

	vector<string> & collectedKeys;
};

// DFS
void innerTraverse(TraversalParams & params)
{
	params.currentArea.visiting = true;
	// collecting keys
	for (size_t i = 0; i < params.currentArea.keys.size(); ++i)
	{
		if (params.currentArea.keys[i].second)
		{
			params.collectedKeys.push_back(params.currentArea.keys[i].first);
		}
	}

	// searching for a path to go through
	for (const AreaLink & i : params.currentAreaLinks)
	{
		bool hasKey = params.collectedKeys.size() == 0;
		for (const string & s : params.collectedKeys)
		{
			if (s == i.unlockedWithKey)
			{
				hasKey = true;
				break;
			}
		}
		// Not visited and we have the key (if we need one in the first place)
		if (!i.toArea->visited && (i.unlockedWithKey == "" || hasKey))
		{
			params.currentArea = params.areas[i.toArea->name];

			params.currentAreaLinks = params.gameMap[i.toArea->name];

			innerTraverse(params);
		}
	}
	params.currentArea.visited = true;
}

// Starting DFS
// throws std::out_of_range if beginning is not an area
void traverse(GameMap & gameMap, Areas & areas, const string & begining)
{
	Area & startArea = areas.at(begining);

	vector<AreaLink> & startAreaLinks = gameMap[begining];

	vector<string> keys;

	TraversalParams params = {
		gameMap,
		areas,
		startArea,
		startAreaLinks,
		keys
	};

	innerTraverse(params);
}

bool areAllVisited(const Areas & areas)
{
	for (auto& i : areas)
	{
		if (i.second.visited == false)
		{
			return false;
		}
	}
	return true;
}

int main()
{
	string str;
	//cin >> str;
	Areas areas;
	GameMap gameMap;

	if (!inp(gameMap, areas, "gamemap.txt"))
	{
		return 1;
	}
	cin >> str; // begin area
	traverse(gameMap, areas, str);
	cout << "Are all visited ? " << (areAllVisited(areas) ? "True." : "False.") << endl;
	return 0;
}