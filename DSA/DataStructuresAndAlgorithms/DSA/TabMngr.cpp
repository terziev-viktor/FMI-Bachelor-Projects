#include "TabMngr.h"
#include <iostream>

unsigned long ID_Generator::next_id = 0;

Tab::Tab()
{
	this->ID = ID_Generator::GetNextID();
	for (size_t i = 0; i < 20; i++)
	{
		this->data.push_back(std::string("FAKE") + std::to_string(this->ID));
	}
}

void Tab::Show(bool details) const
{
	std::cout << "Tab with ID = " << this->ID << std::endl;
	if (details)
	{
		for (auto& i : this->data)
		{
			std::cout << i << std::endl;
		}
	}
	std::cout << "----------------" << std::endl;
}

TabView::TabView()
{
}

TabView::TabView(const vector<shared_ptr<Tab>>& tabs)
{
	for (auto& i : tabs)
	{
		tabRefs.push_back(i);
	}
}

void TabView::ViewAllTabs() const
{
	for (auto& i : tabRefs)
	{
		if (shared_ptr<Tab> ptr = i.lock())
		{
			ptr->Show();
		}
	}
}