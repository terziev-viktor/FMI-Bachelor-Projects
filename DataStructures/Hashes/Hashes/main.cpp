#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>

#include "HashTable.hpp"
#include "Person.h"
#include "HashingFunctions.h"
using std::ifstream;
using std::cout;
using std::cin;
using std::endl;

typedef HashTable<string, Person*, GoodHashingFunction> HashPeople;

// I'm a lazy man, no command hierarchy this time
const size_t COMMANDS_COUNT = 8;
string COMMANDS[COMMANDS_COUNT] = { "RELEASE", "GRAB", "INFO", "ADD", "REMOVE", "SWAP", "PRINT", "EXIT" };

bool InpPeople(const string & path, std::vector<Person*> & people)
{
	ifstream in(path);
	if (!in)
	{
		in.close();
		return false;
	}
	while (true)
	{
		char str_buffer[30];
		in.getline(str_buffer, 30);
		if (!in)
		{
			return true;
		}
		people.push_back(new Person(str_buffer));
	}
	return true;
}

void ReleasePeople(std::vector<Person*> & people)
{
	for (size_t i = 0; i < people.size(); ++i)
	{
		delete people[i];
	}
}

void PrintPeople(const HashPeople & people)
{
	for (HashPeople::Iterator it = people.Begin(); !it.Done(); ++it)
	{
		cout << *((*it)->GetValue()) << endl;
	}
}

void PrintXopo(const Person * first, const Person * p)
{
	cout << p->GetNickname() << endl;
	p = p->Right();
	if (p == first)
	{
		cout << p->GetNickname();
		return;
	}
	PrintXopo(first, p);
}

int main()
{
	srand(time(NULL));
	std::vector<Person*> people;
	HashPeople hash_people;
	if (!InpPeople("file.txt", people))
	{
		cout << "Something went wrong while reading the file" << endl;
		ReleasePeople(people);
		return 1;
	}

	// Setting up the Xopo
	people[0]->SetLeft(people[people.size() - 1]).GrabLeft();
	people[people.size() - 1]->SetRight(people[0]).GrabRight();
	for (size_t i = 0; i < people.size() - 1; ++i)
	{
		people[i]->SetRight(people[i + 1]).GrabRight();
		people[i + 1]->SetLeft(people[i]).GrabLeft();
	}

	// Hashing people
	for (size_t i = 0; i < people.size(); ++i)
	{
		hash_people.Insert(people[i]->GetNickname(), people[i]);
	}
	hash_people.Expand();
	PrintPeople(hash_people);

	// Executing commands
	string inp;
	bool running = true;
	while (running)
	{
		cout << ">";
		cin >> inp;
		size_t i;
		for (i = 0; i < COMMANDS_COUNT; ++i)
		{
			if (COMMANDS[i] == inp)
			{
				break;
			}
		}
		cin.ignore();
		switch (i)
		{
		case 0: // RELEASE
		{
			getline(cin, inp);
			size_t j = inp.find_last_of(' ');
			string side = inp.substr(j + 1);
			string who = inp.substr(0, j);
			Person * * p = hash_people[who];
			if (p == nullptr)
			{
				cout << "There is no person called " << who << " in the xopo!" << endl;
				break;
			}
			if (side == "left")
			{
				(*p)->ReleaseLeft();
			}
			else if (side == "right")
			{
				(*p)->ReleaseRight();
			}
			else
			{
				(*p)->ReleaseLeft().ReleaseRight();
			}
			break;
		}
		case 1: // GRAB
		{
			getline(cin, inp);
			size_t j = inp.find_last_of(' ');
			string side = inp.substr(j + 1);
			string who = inp.substr(0, j);
			Person * * p = hash_people[who];
			if (p == nullptr)
			{
				cout << "There is no person called " << who << " in the xopo!" << endl;
				break;
			}
			if (side == "left")
			{
				(*p)->GrabLeft();
			}
			else if (side == "right")
			{
				(*p)->GrabRight();
			}
			else
			{
				(*p)->GrabLeft().GrabRight();
			}
			break;
		}
		case 2: // INFO
		{
			getline(cin, inp);
			Person * * p = hash_people[inp];
			if (p == nullptr)
			{
				cout << "There is no person called " << inp << " in the xopo!" << endl;
				break;
			}
			cout << *(*p) << endl;
			break;
		}
		case 3: // ADD
		{
			getline(cin, inp);
			size_t first_quote = inp.find_first_of('"');
			size_t second_quote = inp.find_first_of('"', first_quote + 1);
			string who = inp.substr(first_quote + 1, second_quote - 1);

			first_quote = inp.find_first_of('"', second_quote + 1);
			second_quote = inp.find_first_of('"', first_quote + 1);
			string left_label = inp.substr(first_quote + 1, (second_quote - first_quote) - 1);

			first_quote = inp.find_first_of('"', second_quote + 1);
			second_quote = inp.find_first_of('"', first_quote + 1);
			string right_label = inp.substr(first_quote + 1, second_quote - first_quote - 1);

			Person * * left_n = hash_people[left_label];
			if (left_n == nullptr)
			{
				cout << "There is no person called " << left_label << " in the xopo!" << endl;
				break;
			}
			Person * * right_n = hash_people[right_label];
			if (right_n == nullptr)
			{
				cout << "There is no person called " << right_label << " in the xopo!" << endl;
				break;
			}
			Person * p = new Person(who);
			if (!(*left_n)->IsNeibhour(*(*right_n)))
			{
				cout << (*left_n)->GetNickname() << " and " << (*right_n)->GetNickname()
					<< " are not neibhours" << endl;
				break;
			}
			p->SetLeft(*left_n);
			p->SetRight(*right_n);
			(*left_n)->SetRight(p);
			(*right_n)->SetLeft(p);
			hash_people.Insert(who, p);
			people.push_back(p);
			break;
		}
		case 4: // REMOVE
		{
			getline(cin, inp);
			Person * * p1 = hash_people[inp];
			if (p1 == nullptr)
			{
				cout << "There is no person called " << inp << " in the xopo!" << endl;
				break;
			}
			Person * p = *p1;
			if (p->IsFree())
			{
				hash_people.Delete(inp);
				cout << "Free at last!" << endl;
			}
			else
			{
				cout << "This won't be so easy!" << endl;
			}
			break;
		}
		case 5: // SWAP
		{
			getline(cin, inp);
			size_t first_quote = inp.find_first_of('"');
			size_t second_quote = inp.find_first_of('"', first_quote + 1);
			string who = inp.substr(first_quote + 1, second_quote - 1);

			first_quote = inp.find_first_of('"', second_quote + 1);
			second_quote = inp.find_first_of('"', first_quote + 1);
			string with = inp.substr(first_quote + 1, (second_quote - first_quote) - 1);

			HashPeople::KeyValuePair * p1 = hash_people.GetKeyValuePairByKey(who);
			HashPeople::KeyValuePair * p2 = hash_people.GetKeyValuePairByKey(with);
			if (p1 == nullptr)
			{
				cout << "There is no person called " << who << " in the xopo!" << endl;
				break;
			}
			if (p2 == nullptr)
			{
				cout << "There is no person called " << with << " in the xopo!" << endl;
				break;
			}
			Person * personA = p1->GetValue();
			Person * personB = p2->GetValue();
			if (!personA->IsNeibhour(*personB))
			{
				cout << "Can't swap " << who << " with " << with << " because they are not neibhours." << endl;
				break;
			}

			if ((!personA->GrabbedLeft() || personA->Left() == personB)
				&& (!personA->GrabbedRight() || personA->Right() == personB)
				&& (!personB->GrabbedLeft() || personB->Left() == personA)
				&& (!personB->GrabbedRight() || personB->Right() == personA)
				&& (!personA->Left()->GrabbedRight() || personA->Left() == personB)
				&& (!personA->Right()->GrabbedLeft() || personA->Right() == personB)
				&& (!personB->Left()->GrabbedRight() || personB->Left() == personA)
				&& (!personB->Right()->GrabbedLeft() || personB->Right() == personA)) // I feel pain
			{
				personA->SwapNeibhoursWith(*personB); 
				cout << "Swapped " << who << " with " << with << endl;
			}
			else
			{
				cout << "Can't swap " << who << " with " << with << " because they are not free." << endl;
			}
			break;
		}
		case 6: // PRINT
		{
			PrintXopo(people[0], people[0]);
			cout << endl;
			break;
		}
		case 7: // EXIT
		{
			running = false;
			break;
		}
		default:
			cout << "NO SUCH COMMAND" << endl;
			break;
		}
		if (hash_people.Count() < 3)
		{
			running = false;
		}
	}

	ReleasePeople(people);
	return 0;
}