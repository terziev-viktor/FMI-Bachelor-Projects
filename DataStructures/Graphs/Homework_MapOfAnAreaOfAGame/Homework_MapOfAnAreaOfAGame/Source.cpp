#include "GameMap.h"
#include <iostream>
#include <fstream>
using namespace std;

struct TraversalParams
{
	AreaLinks * areaLinks;

	Areas * areas;

	string currentArea;

	vector<string> * collectedKeys;
};

bool inp(AreaLinks & areaLinks, Areas & areas, const string & path)
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
	const AreaLinks::iterator & gend = areaLinks.end();

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

			if (areaLinks.find(areaTo) == gend)
			{
				areaLinks.insert({ areaTo, vector<AreaLink>() });
			}

			string unlockedWithKey = "";
			if (bracketIndex < str.size())
			{
				unlockedWithKey = str.substr(bracketIndex + 1, str.size() - bracketIndex - 3);
			}

			areaLinks[areaFrom].push_back(AreaLink() = {
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

// DFS
void innerTraverse(TraversalParams & params)
{
	Area & currentArea = params.areas->at(params.currentArea);

	vector<AreaLink> & currentAreaLinks = params.areaLinks->at(params.currentArea);

	currentArea.visiting = true;

	// collecting keys
	for (size_t i = 0; i < currentArea.keys.size(); ++i)
	{
		if (currentArea.keys[i].second)
		{
			params.collectedKeys->push_back(currentArea.keys[i].first);
			currentArea.keys[i].second = false; // key is collected

			// I could remember the places where I need a key and go there if the key is collected but this is easier :D
			for (Areas::iterator i = params.areas->begin(); i != params.areas->end(); ++i)
			{
				i->second.visited = false;
			}
		}
	}

	// searching for a path to go through
	for (size_t i = 0; i < currentAreaLinks.size(); ++i)
	{
		bool hasKey = currentAreaLinks[i].unlockedWithKey == "";
		if (!hasKey)
		{
			for (const string & s : *params.collectedKeys)
			{
				if (s == currentAreaLinks[i].unlockedWithKey)
				{
					hasKey = true;
					break;
				}
			}
		}
		
		// Not visited and we have the key (if we need one in the first place)
		if (!currentAreaLinks[i].toArea->visited && !currentAreaLinks[i].toArea->visiting && hasKey)
		{
			params.currentArea = currentAreaLinks[i].toArea->name;

			innerTraverse(params);

			i = 0;
			--i;
		}
	}

	currentArea.visiting = false;
	currentArea.visited = true;
}

// Starting DFS
// throws std::out_of_range if beginning is not an area
void traverse(AreaLinks * AreaLinks, Areas * areas, const string & begining)
{
	vector<string> keys;

	TraversalParams params = {
		AreaLinks,
		areas,
		begining,
		&keys
	};

	innerTraverse(params);
}

bool logDOT(AreaLinks * areaLinks, Areas * areas, const string & path)
{
	ofstream out(path);
	if (!out)
	{
		cerr << "Can't open file " << path << endl;
		return false;
	}

	out << "digraph {\n";

	for (AreaLinks::iterator i = areaLinks->begin(); i != areaLinks->end(); ++i)
	{
		for (size_t j = 0; j < i->second.size(); ++j)
		{
			out << i->first << " -> " << i->second[j].toArea->name << " [label=\"" << i->second[j].unlockedWithKey << "\"];\n";
		}
	}

	for (Areas::iterator i = areas->begin(); i != areas->end(); ++i)
	{
		if (i->second.visited)
		{
			out << i->first << "[label=\"" << i->first;
			for (size_t j = 0; j < i->second.keys.size(); ++j)
			{
				out << " [" << i->second.keys[j].first << ']';
			}
			out << "\"color=green,style=filled];\n";
		}
		else
		{
			out << i->first << "[label=\"" << i->first;
			for (size_t j = 0; j < i->second.keys.size(); ++j)
			{
				out << " [" << i->second.keys[j].first << ']';
			}
			out << "\"color=red,style=filled];\n";
		}
	}

	out << "}\n";
	out.close();
	return true;
}

int main()
{
	string str;
	//cin >> str;
	Areas areas;
	AreaLinks AreaLinks;

	if (!inp(AreaLinks, areas, "gamemap.txt"))
	{
		return 1;
	}
	cin >> str; // start area
	traverse(&AreaLinks, &areas, str);
	
	logDOT(&AreaLinks, &areas, "logdot.txt");

	return 0;
}