#pragma once

#include <vector>
#include <string>
#include <memory>
#include <iostream>
using namespace std;

class ID_Generator
{
public:
	static unsigned long GetNextID()
	{
		return next_id++;
	}

protected:
	static unsigned long next_id;

private:
	ID_Generator() = default;

	ID_Generator(const ID_Generator&) = default;
};

class Tab
{
protected:
	vector<string> data;
	unsigned long ID;
public:
	Tab();
	void Show(bool details = false) const;
};

class TabView
{
public:
	vector<weak_ptr<Tab>> tabRefs;

	TabView();

	TabView(const vector<shared_ptr<Tab>>& tabs);

	void ViewAllTabs() const;
};

class TabMngr
{
public:
	void RegisterTab(shared_ptr<Tab> & ptr)
	{
		cout << ptr << endl;
		tabs.push_back(std::move(ptr));
		cout << ptr << endl;
	}

	void RegisterTab(int count = 1)
	{
		for (size_t i = 0; i < count; i++)
		{
			tabs.push_back(std::make_shared<Tab>());
			// calling the contstuctor with no arguments
		}
	}

	void Swap(size_t a, size_t b)
	{
		if (a != b && a < tabs.size() && b < tabs.size())
		{
			tabs[a].swap(tabs[b]);
		}
		else
		{
			throw "you are an idiot";
		}
	}

	void RemoveLast()
	{
		tabs.erase(tabs.end() - 1);
	}

	inline void SetView(shared_ptr<TabView> & v)
	{
		this->view = v;
	}

	inline void CallView() const
	{
		if (auto ptr = this->view.lock())
		{
			ptr->ViewAllTabs();
		}
		else
		{
			cout << "Previusly set view has died\n";
		}
	}

	inline const vector<shared_ptr<Tab>> & Tabs() const { return tabs; }

protected:
	vector<shared_ptr<Tab>> tabs;
	std::weak_ptr<TabView> view;
};

