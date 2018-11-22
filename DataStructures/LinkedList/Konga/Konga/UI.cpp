#include "UI.h"
using namespace konga_commands;

UI::UI()
{
	// Keeping one konga queue at first
	// User can add more
	Konga konga;
	konga.push_back(Student("Integral4o", UNI::FMI));
	this->kongas.push_back(konga);
}

UI::~UI()
{
	this->commands.delete_elements();
}

void UI::add_command(const command * c)
{
	if (c != nullptr)
	{
		this->commands.keep(c);
	}
}

void UI::execute_command(const string & c)
{
	bool command_found = false;
	for (size_t i = 0; i < this->commands.get_count(); ++i)
	{
		if (this->commands.give(i)->get_trigger() == c)
		{
			try
			{
				std::cout << this->commands.give(i)->execute(this->kongas, this->is_running) << std::endl;
				command_found = true;
				break;
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
				command_found = true;
			}
		}
	}
	if (!command_found)
	{
		std::cout << "Unknown command" << std::endl;
	}
}

void UI::run()
{
	is_running = true;
	string inp;
	while (is_running)
	{
		if (this->kongas.get_count() == 0)
		{
			std::cout << "No more kongas, the only available command is 'quit'" << std::endl;
			std::cin >> inp;
			if (inp == "quit")
			{
				this->execute_command(inp);
			}
		}
		else
		{
			std::cin >> inp;
			this->execute_command(inp);
		}
	}
}

command::command(const string & trigger)
{
	this->set_trigger(trigger);
}

const string & command::get_trigger() const
{
	return this->trigger;
}

void command::set_trigger(const string & trigger)
{
	this->trigger = trigger;
}

string append::execute(dynamic_arr& s, bool&) const
{
	string name, uni_str;
	int line;
	std::cin >> name >> uni_str >> line;
	UNI uni = string_to_UNI(uni_str);
	if (line >= s.get_count())
	{
		throw std::exception("Invalid konga index");
	}
	Konga & konga = s[line];
	const Student & last_st = konga.get_last();
	if (uni == UNI::FMI && (last_st.get_UNI() == UNI::FMI || last_st.get_UNI() == UNI::TU))
	{
		konga.push_back(Student(name, uni));
	}
	else if (uni == UNI::TU && (last_st.get_UNI() == UNI::TU || last_st.get_UNI() == UNI::UNWE))
	{
		konga.push_back(Student(name, uni));
	}
	else if (uni == UNI::UNWE && (last_st.get_UNI() == UNI::UNWE || last_st.get_UNI() == UNI::FMI))
	{
		konga.push_back(Student(name, uni));
	}
	else
	{
		throw std::exception("Incompatible people");
	}
	return "Added student to konga";
}

string remove_last::execute(dynamic_arr& s, bool&) const
{
	int line;
	std::cin >> line;
	Konga & konga = s[line];
	konga.remove_back();
	if (konga.empty())
	{
		s.remove(line);
	}
	return "removed last student from konga";
}

string remove_first::execute(dynamic_arr& s, bool&) const
{
	int line;
	std::cin >> line;
	Konga & konga = s[line];
	konga.remove_front();
	if (konga.empty())
	{
		s.remove(line);
	}
	return "removed first student from konga";
}

string quit::execute(dynamic_arr&, bool& r) const
{
	r = false;
	return "Exiting konga manager";
}

string print::execute(dynamic_arr& s, bool &) const
{
	for (size_t i = 0; i < s.get_count(); ++i)
	{
		std::cout << "line " << i << ": ";
		Konga & konga = s[i];
		for (doubly_linked_list<Student>::iterator it = konga.begin(); !it.is_done();)
		{
			std::cout << *it;
			++it;
			if (!it.is_done())
			{
				std::cout << " <- ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	return "printed all kongas";
}

string remove::execute(dynamic_arr& s, bool &) const
{
	string name;
	int line;
	std::cin >> name >> line;
	Konga & konga = s[line];
	size_t at = 0;
	bool found = false;
	for (doubly_linked_list<Student>::iterator i = konga.begin(); !i.is_done(); ++i)
	{
		if ((*i).get_name() == name)
		{
			found = true;
			break;
		}
		++at;
	}
	if (!found)
	{
		throw std::exception("Student not found");
	}
	Konga cut;
	konga.cut(at, cut);
	if (!cut.empty())
	{
		cut.remove_front();

		if (!cut.empty())
		{
			s.push_back(cut);
		}
	}
	if (konga.empty())
	{
		s.remove(line);
	}
	return "removed student from konga";
}

string konga_commands::merge::execute(dynamic_arr& s, bool &) const
{
	int line1, line2;
	std::cin >> line1 >> line2;
	Konga & konga1 = s[line1];
	Konga & konga2 = s[line2];
	konga1.append(konga2);
	s.remove(line2);
	return "Merged two kongas";
}
